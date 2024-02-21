#include "CDDNS.h"

#ifndef __CDDNSCPP__
#define __CDDNSCPP__
bool CDDNS::ipv6;
std::string CDDNS::tokenHeader;
std::string CDDNS::ipify = "https://api.ipify.org?format=json";
std::string CDDNS::ipType = "A";
std::string CDDNS::lastIp;

bool CDDNS::start()
{
    RReq::RReq ipfy;

    ipfy.Win32Req(ipify.c_str(), {}, true);

    std::string ip = RJSON::RJSON::load(ipfy.Data).get("ip").value;
    if (ip.empty())
    {
        LOG("Ip could not be determined. api.ipify.org did not provide information.", LOG::Error, __FUNCTION__);
        return true;
    }
    if (!ipHasChanged(ip))
    {// ip hasn't changed -> moving on
        return true;
    }
    LOG("Ip has changed to " + ip + ".", LOG::Info, __FUNCTION__);
    LOG("Updating cloudflare...", LOG::Info, __FUNCTION__);
    
    RReq::RReq zonesreq;
    zonesreq.Win32Req("https://api.cloudflare.com/client/v4/zones", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);
    RJSON::JSONElement zones = RJSON::RJSON::load(zonesreq.Data);
    
    if (!zones.get("success").valueAsBool())
    {
        LOG("Error retrieving data from Cloudflare: " + zonesreq.Data, LOG::Error, __FUNCTION__);
        return false;
    }

    std::fstream fs("CloudflareDDNSConfig.json");

    std::string configraw((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement config = RJSON::RJSON::load(configraw);

    for (auto zone : zones.get("result").children)
    {
        for (auto configZone : config.get("zones").children)
        {
            if (configZone["id"].value != zone.get("id").value)
            {
                continue;
            }
            updateZone(configZone);
        }
    }
    LOG("Updated Cloudflare.", LOG::Info, __FUNCTION__);

    return true;
}

void CDDNS::upgradeConfigVersion()
{
    std::fstream fs("CloudflareDDNSConfig.json");

    std::string configRaw((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();
    RJSON::JSONElement config = RJSON::RJSON::load(configRaw);

    if (config["version"].exists())
    {// already newest
        return;
    }
    // version 1.0 -> 1.1
    LOG("Upgrading config to 1.1.", LOG::Info, __FUNCTION__);

    auto& zonesJson = config["zones"];
    std::vector<std::string> zones = zonesJson.asArray();
    
    zonesJson.children.clear();
    for (auto& zone : zones)
    {
        RJSON::JSONElement zoneJson;
        zoneJson.addChild("id", zone);
        RReq::RReq DNSRecordsreq;
        DNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + zone + "/dns_records?&type=" + ipType + "&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

        RJSON::JSONElement DNSRecords = RJSON::RJSON::load(DNSRecordsreq.Data);
        std::vector<std::string> records;
        for (auto DNSrecord : DNSRecords.get("result").children)
        {
            records.emplace_back(DNSrecord.get("id").value);
        }
        zoneJson.addChild("dns_records", records);
        zonesJson.addChild(zoneJson);
    }
}

bool CDDNS::updateZone(RJSON::JSONElement& _zone)
{
    RReq::RReq DNSRecordsreq;
    DNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + _zone["id"].value + "/dns_records", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    RJSON::JSONElement DNSRecords = RJSON::RJSON::load(DNSRecordsreq.Data);
    if (!DNSRecords["success"].valueAsBool())
    {
        LOG("Failed to get DNS records: " + DNSRecords["error"].value, LOG::Error, __FUNCTION__);
        return false;
    }

    for (auto DNSRecord : DNSRecords.get("result").children)
    {
        RJSON::JSONElement dns_records = _zone["dns_records"];
        if (std::find_if(dns_records.children.begin(), dns_records.children.end(), [&DNSRecord](RJSON::JSONElement& _record)-> bool {
            if (DNSRecord["id"].value == _record.value)
            {
                return true;
            }
            }) == std::end(dns_records.children))
        {
            continue;
        }
        
        DNSRecord.get("content").value = lastIp;
        std::string newRecords = DNSRecord.asJSON();
        RReq::RReq UpdateDNSRecordsreq;
        UpdateDNSRecordsreq.Win32Req(
            "https://api.cloudflare.com/client/v4/zones/" + _zone["id"].value + "/dns_records/" + DNSRecord.get("id").value,
            RReq::PUTRequest, { tokenHeader , "Content-Type:application/json" }, newRecords, true);
        RJSON::JSONElement UpdateDNSRecords = RJSON::RJSON::load(UpdateDNSRecordsreq.Data);

        if (!UpdateDNSRecords.get("success").valueAsBool())
        {
            LOG("Error sending data to Cloudflare: " + UpdateDNSRecordsreq.Data, LOG::Error, __FUNCTION__);
            return false;
        }
    }
}

bool  CDDNS::ipHasChanged(std::string _currentIp)
{
#ifdef _DEBUG
    return true;
#endif // _DEBUG
    if (lastIp == _currentIp)
    {
        return true;
    }
    // new ip
    if (!std::filesystem::exists("last.ip"))
    {
        std::ofstream of("last.ip");
        of << _currentIp;
        of.close();
        return true;
    }

    lastIp = _currentIp;
    std::fstream fs("last.ip");
    fs.close();

    fs.open("last.ip", std::ios::out | std::ios::trunc);// empty file
    fs << _currentIp;
    fs.close();
    return true;
}

bool CDDNS::loadConfig()
{
    /*v1.0
    {
        token: "",
        "IPv6": "True",
        "autoupdate": "True",
        "autostart": "True", 
        "asService": "False", 
        "serviceInterval": "10",
        zones: [
            "zone id"
        ]
    }
    v1.1
    {
        version: "1.1",
        token: "",
        "IPv6": "True",
        "IPv4": "True",
        "IPauto": "True",
        "autoupdate": "True",
        "autostart": "True",
        "asService": "False",
        zones: [
            {
                id: "zone id",
                dns_records: [
                    "DNS record id"
                ]
            }
        ]
    }
    */
    if (!std::filesystem::exists("CloudflareDDNSConfig.json"))
    {
        std::ofstream("CloudflareDDNSConfig.json");
    }
    upgradeConfigVersion();


    std::fstream fs("CloudflareDDNSConfig.json");

    std::string configraw((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement config = RJSON::RJSON::load(configraw);

    tokenHeader = "Authorization: Bearer " + config.get("token").value;

    if (config["IPv6"].valueAsBool())
    {
        ipify = "https://api64.ipify.org/?format=json";
        ipType = "AAAA";
    }
    return true;
}

#endif //!__CDDNSCPP__
#include "CDDNS.h"

#ifndef __CDDNSCPP__
#define __CDDNSCPP__
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
        for (auto allowedzone : config.get("zones").children)
        {
            if (allowedzone.value != zone.get("id").value)
            {
                continue;
            }
            RReq::RReq DNSRecordsreq;
            DNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + zone.get("id").value + "/dns_records?&type=" + ipType + "&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

            RJSON::JSONElement DNSRecords = RJSON::RJSON::load(DNSRecordsreq.Data);
            for (auto DNSrecord : DNSRecords.get("result").children)
            {
                DNSrecord.get("content").value = ip;
                std::string newRecords = DNSrecord.asJSON();
                RReq::RReq UpdateDNSRecordsreq;
                UpdateDNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + zone.get("id").value + "/dns_records/" + DNSrecord.get("id").value, 
                    RReq::PUTRequest, { tokenHeader , "Content-Type:application/json" }, newRecords, true);
                RJSON::JSONElement UpdateDNSRecords = RJSON::RJSON::load(UpdateDNSRecordsreq.Data);

                if (!UpdateDNSRecords.get("success").valueAsBool())
                {
                    LOG("Error sending data to Cloudflare: " + UpdateDNSRecordsreq.Data, LOG::Error, __FUNCTION__);
                    return false;
                }
            }
        }
    }
    LOG("Updated cloudflare.", LOG::Info, __FUNCTION__);

    return true;
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
    /*
    {
        token: "",
        zones: [
            "zone id"
        ]
    }
    */
    if (!std::filesystem::exists("CloudflareDDNSConfig.json"))
    {
        LOG("Couldn't find CloudflareDDNSConfig.json.", LOG::Error, __FUNCTION__);

        return false;
    }
    
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
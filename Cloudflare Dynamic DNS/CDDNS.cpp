#include "CDDNS.h"

#ifndef __CDDNSCPP__
#define __CDDNSCPP__
std::string CDDNS::tokenHeader;

bool CDDNS::start()
{
    RReq::RReq ipfy;
    ipfy.Win32Req("https://api.ipify.org?format=json", {}, true);

    std::string ip = RJSON::RJSON::load(ipfy.Data).get("ip").value;
    if (ip.empty())
    {
        LOG("Ip could not be determined. api.ipify.org did not provide information.", LOG::Error, __FUNCTION__);
        return true;
    }
    if (ipHasChanged(ip))
    {
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
                if (allowedzone.value == zone.get("id").value)
                {
                    RReq::RReq DNSRecordsreq;
                    DNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + zone.get("id").value + "/dns_records?&type=A&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

                    RJSON::JSONElement DNSRecords = RJSON::RJSON::load(DNSRecordsreq.Data);
                    for (auto DNSrecord : DNSRecords.get("result").children)
                    {//
                        DNSrecord.get("content").value = ip;
                        std::string s = DNSrecord.rawString();
                        RReq::RReq UpdateDNSRecordsreq;
                        UpdateDNSRecordsreq.Win32Req("https://api.cloudflare.com/client/v4/zones/" + zone.get("id").value + "/dns_records/" + DNSrecord.get("id").value, RReq::PUTRequest, { tokenHeader , "Content-Type:application/json" },
                            s, true);
                        RJSON::JSONElement UpdateDNSRecords = RJSON::RJSON::load(UpdateDNSRecordsreq.Data);

                        if (!UpdateDNSRecords.get("success").valueAsBool())
                        {
                            LOG("Error sending data to Cloudflare: " + UpdateDNSRecordsreq.Data, LOG::Error, __FUNCTION__);
                            break;
                        }
                    }
                }
            }
        }
        LOG("Updated cloudflare.", LOG::Info, __FUNCTION__);
    }
    return true;
}

bool  CDDNS::ipHasChanged(std::string _currentIp)
{
    return true;
    if (!std::filesystem::exists("last.ip"))
    {
        std::ofstream("last.ip");
    }

    std::fstream fs;
    fs.open("last.ip");

    std::string lastip((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    if (lastip != _currentIp)
    {// new ip
        fs.open("last.ip", std::ios::out | std::ios::trunc);
        fs << _currentIp;
        fs.close();
        return true;
    }
    return false;
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
    return true;
}

#endif //!__CDDNSCPP__
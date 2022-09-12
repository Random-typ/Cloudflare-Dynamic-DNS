#include <iostream>
#include <Windows.h>
#include "RJSON.h"
#include "RReq.h"
#include "Logger.h"
std::string tokenHeader;;

bool ipHasChanged(std::string _currentIp)
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

void loadConfig()
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

        exit(1);
    }
    std::fstream fs("CloudflareDDNSConfig.json");

    std::string configraw((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement config = RJSON::RJSON::load(configraw);

    tokenHeader = "Authorization: Bearer " + config.get("token").value;
}


//int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
int main()
{
    //std::string str1 = "{\"result\":[{\"id\":\"a5acf5ed6859954c617df733fef5e6a2\",\"name\":\"aquamarinband.de\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":null,\"original_registrar\":null,\"original_dnshost\":null,\"modified_on\":\"2022 - 09 - 08T21:02 : 47.913937Z\",\"created_on\":\"2022 - 09 - 07T12:47 : 25.834488Z\",\"activated_on\":\"2022 - 09 - 07T12:53 : 35.449762Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":\"3af9cc45c3d647af1a89e15f4593bd8d\",\"type\":\"user\",\"email\":\"no.name.typ2.0@gmail.com\"},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"no.name.typ2.0@gmail.com\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#access:edit\",\"#access : read\",\"#analytics : read\",\"#app : edit\",\"#auditlogs : read\",\"#billing : edit\",\"#billing : read\",\"#blocks : edit\",\"#blocks : read\",\"#cache_purge : edit\",\"#dash_sso : edit\",\"#dash_sso : read\",\"#dns_records : edit\",\"#dns_records : read\",\"#fbm_acc : edit\",\"#fbm : edit\",\"#fbm : read\",\"#healthchecks : edit\",\"#healthchecks : read\",\"#http_applications : edit\",\"#http_applications : read\",\"#image : edit\",\"#image : read\",\"#lb : edit\",\"#lb : read\",\"#legal : edit\",\"#legal : read\",\"#logs : edit\",\"#logs : read\",\"#magic : edit\",\"#magic : read\",\"#member : edit\",\"#member : read\",\"#organization : edit\",\"#organization : read\",\"#ssl : edit\",\"#ssl : read\",\"#stream : edit\",\"#stream : read\",\"#subscription : edit\",\"#subscription : read\",\"#teams : edit\",\"#teams : pii\",\"#teams : read\",\"#teams : report\",\"#waf : edit\",\"#waf : read\",\"#waitingroom : edit\",\"#waitingroom : read\",\"#webhooks : edit\",\"#webhooks : read\",\"#worker : edit\",\"#worker : read\",\"#zaraz : edit\",\"#zaraz : read\",\"#zone : edit\",\"#zone : read\",\"#zone_settings : edit\",\"#zone_settings : read\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}},{\"id\":\"d2e5eb1888a443d707899146bc4bacb0\",\"name\":\"funkmates.de\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":null,\"original_registrar\":null,\"original_dnshost\":null,\"modified_on\":\"2022 - 03 - 22T19 : 12 : 29.051232Z\",\"created_on\":\"2022 - 03 - 22T18 : 56 : 20.213769Z\",\"activated_on\":\"2022 - 03 - 22T19 : 12 : 29.051232Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":\"3af9cc45c3d647af1a89e15f4593bd8d\",\"type\":\"user\",\"email\":\"no.name.typ2.0@gmail.com\"},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"no.name.typ2.0@gmail.com\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#access:edit\",\"#access : read\",\"#analytics : read\",\"#app : edit\",\"#auditlogs : read\",\"#billing : edit\",\"#billing : read\",\"#blocks : edit\",\"#blocks : read\",\"#cache_purge : edit\",\"#dash_sso : edit\",\"#dash_sso : read\",\"#dns_records : edit\",\"#dns_records : read\",\"#fbm_acc : edit\",\"#fbm : edit\",\"#fbm : read\",\"#healthchecks : edit\",\"#healthchecks : read\",\"#http_applications : edit\",\"#http_applications : read\",\"#image : edit\",\"#image : read\",\"#lb : edit\",\"#lb : read\",\"#legal : edit\",\"#legal : read\",\"#logs : edit\",\"#logs : read\",\"#magic : edit\",\"#magic : read\",\"#member : edit\",\"#member : read\",\"#organization : edit\",\"#organization : read\",\"#ssl : edit\",\"#ssl : read\",\"#stream : edit\",\"#stream : read\",\"#subscription : edit\",\"#subscription : read\",\"#teams : edit\",\"#teams : pii\",\"#teams : read\",\"#teams : report\",\"#waf : edit\",\"#waf : read\",\"#waitingroom : edit\",\"#waitingroom : read\",\"#webhooks : edit\",\"#webhooks : read\",\"#worker : edit\",\"#worker : read\",\"#zaraz : edit\",\"#zaraz : read\",\"#zone : edit\",\"#zone : read\",\"#zone_settings : edit\",\"#zone_settings : read\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}},{\"id\":\"e7bfeb7bfc316dc8f99aa46fe8f6ce12\",\"name\":\"retard - inc.com\",\"status\":\"active\",\"paused\":false,\"type\":\"full\",\"development_mode\":0,\"name_servers\":[\"agustin.ns.cloudflare.com\",\"karsyn.ns.cloudflare.com\"],\"original_name_servers\":[\"dns1.registrar - servers.com\",\"dns2.registrar - servers.com\"],\"original_registrar\":\"namecheap, inc. (id : 1068)\",\"original_dnshost\":null,\"modified_on\":\"2021 - 08 - 30T16:15 : 15.839845Z\",\"created_on\":\"2021 - 08 - 30T15:11 : 45.008972Z\",\"activated_on\":\"2021 - 08 - 30T16:06 : 12.622417Z\",\"meta\":{\"step\":2,\"custom_certificate_quota\":0,\"page_rule_quota\":3,\"phishing_detected\":false,\"multiple_railguns_allowed\":false},\"owner\":{\"id\":\"3af9cc45c3d647af1a89e15f4593bd8d\",\"type\":\"user\",\"email\":\"no.name.typ2.0@gmail.com\"},\"account\":{\"id\":\"fa5eb97b53d289e16d7e60f4fde15563\",\"name\":\"no.name.typ2.0@gmail.com\"},\"tenant\":{\"id\":null,\"name\":null},\"tenant_unit\":{\"id\":null},\"permissions\":[\"#access:edit\",\"#access : read\",\"#analytics : read\",\"#app : edit\",\"#auditlogs : read\",\"#billing : edit\",\"#billing : read\",\"#blocks : edit\",\"#blocks : read\",\"#cache_purge : edit\",\"#dash_sso : edit\",\"#dash_sso : read\",\"#dns_records : edit\",\"#dns_records : read\",\"#fbm_acc : edit\",\"#fbm : edit\",\"#fbm : read\",\"#healthchecks : edit\",\"#healthchecks : read\",\"#http_applications : edit\",\"#http_applications : read\",\"#image : edit\",\"#image : read\",\"#lb : edit\",\"#lb : read\",\"#legal : edit\",\"#legal : read\",\"#logs : edit\",\"#logs : read\",\"#magic : edit\",\"#magic : read\",\"#member : edit\",\"#member : read\",\"#organization : edit\",\"#organization : read\",\"#ssl : edit\",\"#ssl : read\",\"#stream : edit\",\"#stream : read\",\"#subscription : edit\",\"#subscription : read\",\"#teams : edit\",\"#teams : pii\",\"#teams : read\",\"#teams : report\",\"#waf : edit\",\"#waf : read\",\"#waitingroom : edit\",\"#waitingroom : read\",\"#webhooks : edit\",\"#webhooks : read\",\"#worker : edit\",\"#worker : read\",\"#zaraz : edit\",\"#zaraz : read\",\"#zone : edit\",\"#zone : read\",\"#zone_settings : edit\",\"#zone_settings : read\"],\"plan\":{\"id\":\"0feeeeeeeeeeeeeeeeeeeeeeeeeeeeee\",\"name\":\"Free Website\",\"price\":0,\"currency\":\"USD\",\"frequency\":\"\",\"is_subscribed\":false,\"can_subscribe\":false,\"legacy_id\":\"free\",\"legacy_discount\":false,\"externally_managed\":false}}],\"result_info\":{\"page\":1,\"per_page\":20,\"total_pages\":1,\"count\":3,\"total_count\":3},\"success\":true,\"errors\":[],\"messages\":[]}";
    //std::string str2 = "{\"result\":[{\"id\":\"id1\",\"name\":\"domain1.com\"},{\"id\":\"id2\",\"name\":\"domain2.com\"}],\"info\":\"information\"}";
    //RJSON::JSONElement elele = RJSON::RJSON::load(str1);
    //RReq::RReq cloudflare;
    //cloudflare.Win32Req("https://api.cloudflare.com/client/v4/zones/e7bfeb7bfc316dc8f99aa46fe8f6ce12/dns_records?type=A&name=example.com&content=127.0.0.1&proxied=false&page=1&per_page=100&order=type&direction=desc&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    Logger::setupLogfile("logs");
    loadConfig();

    for (;; Sleep(1000*60*10))//10 Minutes
    {
        RReq::RReq ipfy;
        ipfy.Win32Req("https://api.ipify.org?format=json", {}, true);

        std::string ip = RJSON::RJSON::load(ipfy.Data).get("ip").value;
        if (ip.empty())
        {
            LOG("Ip could not be determined. api.ipify.org did not provide information.", LOG::Error, __FUNCTION__);
            continue;
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
                break;
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
    }
}

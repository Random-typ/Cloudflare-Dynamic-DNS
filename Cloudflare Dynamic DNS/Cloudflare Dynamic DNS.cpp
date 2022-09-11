#include <iostream>
#include <Windows.h>
#include "RJSON.h"
#include "RReq.h"
#include "Logger.h"
std::string tokenHeader = "Authorization: Bearer 5O9DnwVIhZLfaILJQpngWYgrsZuYWbOVQmiq0dpc";

bool ipHasChanged(std::string _currentIp)
{
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

        exit(1);
    }
    std::fstream fs("CloudflareDDNSConfig.json");

    std::string configraw((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    fs.close();

    RJSON::JSONElement config = RJSON::RJSON::load(configraw);

    if (!config.hasChild("token"))
    {
        std::cout << "Cloudflare API token: \n";
        std::string token;
        std::cin >> token;

        config.addChild("token", token);
    }
    if (!config.hasChild("zones"))
    {
        std::cout << "Enter zone id's: \n";
        RJSON::JSONElement elem;
        elem.name = "zones";
        elem.type = RJSON::Array;
        std::string id;

        std::getline(std::cin, id);
        while (true)
        {
            id.clear();

            std::getline(std::cin, id);
            if (id.empty())
                break;
            
            elem.addChild("", id);
        }


        config.addChild(elem);
    }

    std::fstream fscofnig("CloudflareDDNSConfig.json");

    fscofnig << config.rawString();
    fscofnig.close();
}


//int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
int main()
{
    //RReq::RReq cloudflare;
    //cloudflare.Win32Req("https://api.cloudflare.com/client/v4/zones/e7bfeb7bfc316dc8f99aa46fe8f6ce12/dns_records?type=A&name=example.com&content=127.0.0.1&proxied=false&page=1&per_page=100&order=type&direction=desc&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    //std::cout << cloudflare.Data << "\n\n";


    Logger::setupLogfile("logs");
    loadConfig();

    for (;; Sleep(1000*60*10))//10 Minutes
    {
        RReq::RReq ipfy;
        ipfy.Win32Req("https://api.ipify.org?format=json", {}, true);

        RJSON::JSONElement elem = RJSON::RJSON::load(ipfy.Data).get("ip");
        if (elem.value.empty())
        {
            LOG("Ip could not be determined. api.ipify.org did not provide information.", LOG::Error, __FUNCTION__);
            continue;
        }
        if (ipHasChanged(elem.value))
        {
            LOG("Ip has changed to " + elem.value + ".", LOG::Info, __FUNCTION__);
            LOG("Updating cloudflare.", LOG::Info, __FUNCTION__);



            




        }
    }
}

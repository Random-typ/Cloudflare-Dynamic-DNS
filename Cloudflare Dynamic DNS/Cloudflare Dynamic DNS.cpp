#include <iostream>
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


int main()
{
    RReq::RReq cloudflare;
    cloudflare.Win32Req("https://api.cloudflare.com/client/v4/user", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    std::cout << cloudflare.Data << "\n\n";

    cloudflare.Data.clear();
    cloudflare.Win32Req("https://api.cloudflare.com/client/v4/accounts?page=1&per_page=20&direction=desc&name=RandomTyp", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    std::cout << cloudflare.Data << "\n\n";

    cloudflare.Data.clear();
    cloudflare.Win32Req("https://api.cloudflare.com/client/v4/zones?name=retard-inc.com&status=active&account.id=3af9cc45c3d647af1a89e15f4593bd8d&account.name=id&page=1&per_page=20&order=status&direction=desc&match=any", RReq::GETRequest, { tokenHeader , "Content-Type:application/json" }, "", true);

    std::cout << cloudflare.Data << "\n\n";;
    
    
    Logger::setupLogfile("logs");

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

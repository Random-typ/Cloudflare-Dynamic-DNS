#include <iostream>
#include <Windows.h>
#include <thread>
//  https://github.com/Random-typ/RJSON
#include "RJSON.h"
#include "RReq.h"
#include "Logger.h"

#ifdef _DEBUG
#pragma comment(lib, "RJSON_d.lib")
#else
#pragma comment(lib, "RJSON.lib")
#endif // _DEBUG



#ifndef __CDDNS__
#define __CDDNS__
class CDDNS
{
public:
    // @returns true on error
    static bool start();
    static void upgradeConfigVersion();

    static bool updateZone(RJSON::JSONElement& _zone);
    // returns true if ip has changed. Sets lastIp to current ip if ip changed
    static bool ipHasChanged(std::string _currentIp);

    static bool loadConfig();

    static bool ipv6;
    static std::string tokenHeader;
    static std::string ipify;
    static std::string ipType;
    static std::string lastIp;
}; 
#endif // !__CDDNS__
#include <iostream>
#include <Windows.h>
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
    static bool start();
    // returns true if ip has changed
    static bool ipHasChanged(std::string _currentIp);

    static bool loadConfig();

    static std::string tokenHeader;
    static std::string ipify;
    static std::string ipType;
    static std::string lastIp;
}; 
#endif // !__CDDNS__
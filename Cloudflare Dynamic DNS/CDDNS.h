#include <iostream>
#include <Windows.h>
//  https://github.com/Random-typ/RJSON
#include "RJSON.h"
#include "RReq.h"
#include "Logger.h"

#ifndef __CDDNS__
#define __CDDNS__
class CDDNS
{
public:
    static bool start();

    static bool ipHasChanged(std::string _currentIp);

    static bool loadConfig();

    static std::string tokenHeader;
    static std::string ipify;
    static std::string ipType;
}; 
#endif // !__CDDNS__
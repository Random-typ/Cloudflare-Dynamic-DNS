#include <iostream>
#include <Windows.h>
#include "RJSON.h"
#include "RReq.h"
#include "Logger.h"

#ifndef __CDDNS__
#define __CDDNS__
class CDDNS
{
public:
    static std::string tokenHeader;

    static bool start();

    static bool ipHasChanged(std::string _currentIp);

    static bool loadConfig();

}; 
#endif // !__CDDNS__
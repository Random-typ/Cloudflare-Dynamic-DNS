#include "CDDNS.h"

#ifdef NOWINDOW
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif // NOWINDOW
{
    Logger::setupLogfile("logs");

    if (!CDDNS::loadConfig())
    {
        LOG("Failed to load config.", LOG::Error, __FUNCTION__);
    }

    for (;; Sleep(1000*60*10))//10 Minutes
    {
        if (!CDDNS::start())
        {
            break;
        }
    }
}

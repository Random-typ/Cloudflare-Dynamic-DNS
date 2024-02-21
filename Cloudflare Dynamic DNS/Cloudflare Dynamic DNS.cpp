#include "CDDNS.h"

#ifdef NOWINDOW
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main()
#endif // NOWINDOW
{
    LOG("Starting...", LOG::Info, __FUNCTION__);
    Logger::setupLogfile("logs");

    if (!CDDNS::loadConfig())
    {
        LOG("Failed to load config.", LOG::Error, __FUNCTION__);
    }

    for (;;)
    {
        if (!CDDNS::start())
        {// Failed
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    //LOG("Program closing.", LOG::Info, __FUNCTION__);
    return 0;
}

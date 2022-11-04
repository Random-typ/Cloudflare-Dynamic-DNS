#include <CDDNS.h>

#include "Service.cpp"









#ifdef NOWINDOW
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char* argv[])
#endif // NOWINDOW
{
    std::ofstream why("C:/Users/Random-Typ/source/repos/Cloudflare Dynamic DNS/x64/Debug/file");
    if (argc == 1)
    {
        std::string str = argv[0];
        str = str.substr(0, str.find_last_of("\\/"));
        SetCurrentDirectoryA(str.c_str());
    }
    why << std::filesystem::current_path();
    why.close();

    SERVICE_TABLE_ENTRYA DispatchTable[] =
    {
        { (LPSTR)"Cloudflare Dynamic DNS", (LPSERVICE_MAIN_FUNCTIONA)ServiceMain },
        { NULL, NULL }
    };

    
    if (!StartServiceCtrlDispatcherA(DispatchTable))
    {
        std::ofstream of("file");
        of << GetLastError();
        std::cout << GetLastError();
        of.close();
        return 1;
    }
	return 0;
}
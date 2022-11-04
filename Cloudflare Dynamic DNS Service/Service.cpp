#include <Windows.h>
#include <CDDNS.h>

static SERVICE_STATUS_HANDLE statusHandle;

static void WINAPI ServiceHandler(DWORD _dwControl);

static SC_HANDLE getSCMDatabaseHandle(DWORD _dwDesiredAccess);

static void ServiceMain();

static void ServiceInstall();

static void ServiceInit();

static void reportServiceStatus(DWORD _dwCurrentState, DWORD _dwWin32ExitCode, DWORD _dwWaitHin);


VOID WINAPI ServiceHandler(DWORD dwCtrl)
{
    // Handle the requested control code. 

    switch (dwCtrl)
    {
    case SERVICE_CONTROL_STOP:
        reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);


        return;

    case SERVICE_CONTROL_INTERROGATE:
        break;

    default:
        break;
    }

}

SC_HANDLE getSCMDatabaseHandle(DWORD _dwDesiredAccess) {
	SC_HANDLE scHandle = OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASE, _dwDesiredAccess);

	if (!scHandle)
	{
		return NULL;
	}

	return scHandle;
}

void ServiceMain() {

	statusHandle = RegisterServiceCtrlHandlerA("Cloudflare Dynamic DNS", ServiceHandler);
	if (!statusHandle)
	{
		return;
	}
	reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    ServiceInit();
}
void ServiceInstall() {
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    std::string path;
    path.resize(MAX_PATH);

    if (!GetModuleFileNameA(NULL, path.data(), MAX_PATH))
    {
        printf("Cannot install service (%d)\n", GetLastError());
        return;
    }


    path = "\"" + path + "\"";



    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    // Create the service
    //C:\Users\Random-Typ\source\repos\Cloudflare Dynamic DNS\x64\Debug
    schService = CreateServiceA(
        schSCManager,              // SCM database 
        "Cloudflare Dynamic DNS",  // name of service 
        "Cloudflare Dynamic DNS",  // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        path.c_str(),              // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}
void ServiceInit() {
	HANDLE event = CreateEventA(NULL, true, false, NULL);
	if (!event)
	{
		reportServiceStatus(SERVICE_STOPPED, GetLastError(), 0);
		return;
	}

	reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 3000);


	Logger::setupLogfile("logs");
    if (!CDDNS::loadConfig())
    {
        LOG("Failed to load config.", LOG::Error, __FUNCTION__);
    }

	bool cddns = CDDNS::start();

	if (Logger::fileStream.is_open())
	{
		Logger::fileStream.close();
	}

	if (!cddns)
	{
		reportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
		return;
	}

	reportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

// _dwCurrentState
//  Allowed values:
//    SERVICE_CONTINUE_PENDING	The service continue is pending.
//    SERVICE_PAUSE_PENDING		The service pause is pending.
//    SERVICE_PAUSED			The service is paused.
//    SERVICE_RUNNING			The service is running.
//    SERVICE_START_PENDING     The service is starting.
//    SERVICE_STOP_PENDING      The service is stopping.
//    SERVICE_STOPPED           The service is not running.
//
//
// _dwWin32ExitCode
//    NO_ERROR 
//
void reportServiceStatus(DWORD _dwCurrentState, DWORD _dwWin32ExitCode, DWORD _dwWaitHin) {
	SERVICE_STATUS status;

	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

	status.dwCurrentState = _dwCurrentState;

	status.dwControlsAccepted = SERVICE_ACCEPT_STOP; // maybe edit this

	status.dwWin32ExitCode = _dwWin32ExitCode;

	status.dwServiceSpecificExitCode = NULL;

	status.dwCheckPoint = 0;

	status.dwWaitHint = _dwWaitHin;

    if (SetServiceStatus(statusHandle, &status))
    {

    }
	
}

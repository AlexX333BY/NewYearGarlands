#ifdef _DEBUG 
#include "GarlandServer.h"

using namespace NewYearGarlands;

int main()
{
	GarlandServer *server = new GarlandServer();
	BOOL bStartupResult = server->Start();
	printf("Is server started? %s", bStartupResult ? "TRUE" : "FALSE");
	system("pause");
	if (bStartupResult) {
		server->Shutdown();
	}
	return 0;
}
#else
#include <stdio.h>
#include <Windows.h>
#include "GarlandService.h"

const LPCTSTR lpcInstallArgument = "-i";

BOOL InstallService()
{
	SC_HANDLE hSCManager;
	SC_HANDLE hService;
	TCHAR lptsPath[MAX_PATH];

	if (!GetModuleFileName(NULL, lptsPath, MAX_PATH))
	{
		return FALSE;
	}

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (NULL == hSCManager)
	{
		return FALSE;
	}

	hService = CreateService(hSCManager, NewYearGarlands::lpcsServiceName, NewYearGarlands::lpcsServiceDescription, SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		lptsPath, NULL, NULL, NULL, ".\\LocalSystem", NULL);

	if (hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return FALSE;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return TRUE;
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		if (strcmp(argv[1], lpcInstallArgument) == 0)
		{
			if (InstallService())
			{
				printf("Succesfully installed\n");
			}
			else
			{
				printf("Error while installing service: %d\n", GetLastError());
			}
		}
		else
		{
			printf("Unknown argument: %s\n", argv[1]);
			return 3;
		}
	}
	else if (argc > 2)
	{
		printf("Cannot process so much arguments\n");
		return 2;
	}
	else
	{
		return NewYearGarlands::StartGarlandService() ? 0 : 1;
	}

	return 0;
}
#endif

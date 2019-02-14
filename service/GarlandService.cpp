#include "GarlandService.h"
#include "GarlandServer.h"

namespace NewYearGarlands
{
	SERVICE_STATUS_HANDLE ghServiceStatusHandle;
	SERVICE_STATUS gssServiceStatus;
	HANDLE ghStopEvent;

	VOID ReportServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint, DWORD dwCheckpoint)
	{
		gssServiceStatus.dwCurrentState = dwCurrentState;
		gssServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
		gssServiceStatus.dwWaitHint = dwWaitHint;

		if (dwCurrentState == SERVICE_START_PENDING)
		{
			gssServiceStatus.dwControlsAccepted = 0;
		}
		else
		{
			gssServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
		}

		SetServiceStatus(ghServiceStatusHandle, &gssServiceStatus);
	}

	VOID WINAPI ServiceCtrlHandler(DWORD dwCtrl)
	{
		static GarlandServer *gsServer = new GarlandServer();

		switch (dwCtrl)
		{
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 0);
			gsServer->Shutdown();
			ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 1);
			delete gsServer;
			SetEvent(ghStopEvent);
			return;
		default:
			break;
		}
	}

	VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
	{
		ghServiceStatusHandle = RegisterServiceCtrlHandler(lpcsServiceName, ServiceCtrlHandler);

		if (ghServiceStatusHandle == 0)
		{
			return;
		}

		gssServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

		ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 0);

		ghStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 0, 1);

		if (ghStopEvent == NULL)
		{
			ReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0, 0);
			return;
		}

		ReportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0, 0);

		WaitForSingleObject(ghStopEvent, INFINITE);

		ReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0, 0);
	}

	BOOL StartGarlandService()
	{
		int iServiceNameLength = lstrlen(lpcsServiceName);
		TCHAR *szServiceName = (TCHAR *)calloc(iServiceNameLength + 1, sizeof(TCHAR));

		SERVICE_TABLE_ENTRY DispatchTable[] =
		{
			{ szServiceName, ServiceMain },
			{ NULL, NULL }
		};

		return StartServiceCtrlDispatcher(DispatchTable);
	}
}

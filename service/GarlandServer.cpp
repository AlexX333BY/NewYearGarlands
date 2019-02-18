#include "GarlandServer.h"
#include "MessageGeneratorProvider.h"
#include "..\common\ServerGarlandPipe.h"

namespace NewYearGarlands
{
	DWORD WINAPI ClientWaitingThreadProc(GarlandServerThreadArgument *pgstaArgument);
	DWORD WINAPI LightingThreadProc(GarlandServerThreadArgument *pgstaArgument);

	GarlandServer::GarlandServer(DWORD dwPause) : m_bIsRunning(FALSE),
		m_hClientListeningThread(INVALID_HANDLE_VALUE), m_hLightingThread(INVALID_HANDLE_VALUE)
	{
		m_gstaArgument.m_dwLightSleepTime = dwPause;
	}

	GarlandServer::GarlandServer() : m_bIsRunning(FALSE),
		m_hClientListeningThread(INVALID_HANDLE_VALUE), m_hLightingThread(INVALID_HANDLE_VALUE)
	{
		m_gstaArgument.m_dwLightSleepTime = 1000;
	}

	GarlandServer::~GarlandServer()
	{
		if (IsRunning())
		{
			Shutdown();
		}
	}

	BOOL GarlandServer::IsRunning()
	{
		return m_bIsRunning;
	}

	BOOL GarlandServer::Start()
	{
		if (!IsRunning())
		{
			m_gstaArgument.m_bShouldRun = TRUE;
			m_hClientListeningThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientWaitingThreadProc, &m_gstaArgument, 0, NULL);
			if (m_hClientListeningThread != NULL)
			{
				m_hLightingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LightingThreadProc, &m_gstaArgument, 0, NULL);
				if (m_hLightingThread != NULL)
				{
					m_bIsRunning = TRUE;
				}
				else
				{
					m_gstaArgument.m_bShouldRun = FALSE;
					CancelSynchronousIo(m_hClientListeningThread);
					m_hLightingThread = INVALID_HANDLE_VALUE;
					WaitForSingleObject(m_hClientListeningThread, INFINITE);
					CloseHandle(m_hClientListeningThread);
					m_hClientListeningThread = INVALID_HANDLE_VALUE;
				}
			}
			else
			{
				m_hClientListeningThread = INVALID_HANDLE_VALUE;
			}

			return IsRunning();
		}
		else
		{
			return FALSE;
		}
	}

	BOOL GarlandServer::Shutdown()
	{
		if (IsRunning())
		{
			m_gstaArgument.m_bShouldRun = FALSE;
			CancelSynchronousIo(m_hClientListeningThread);
			SetEvent(m_gstaArgument.m_hNonZeroCountEvent);

			WaitForSingleObject(m_hClientListeningThread, INFINITE);
			CloseHandle(m_hClientListeningThread);
			WaitForSingleObject(m_hLightingThread, INFINITE);
			CloseHandle(m_hLightingThread);

			for (std::list<ServerGarlandPipe *>::iterator it = m_gstaArgument.m_lClientPipes.begin();
				it != m_gstaArgument.m_lClientPipes.end(); ++it)
			{
				(*it)->Disconnect();
				delete *it;
			}
			m_gstaArgument.m_lClientPipes.clear();
			m_bIsRunning = FALSE;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	DWORD WINAPI ClientWaitingThreadProc(GarlandServerThreadArgument *pgstaArgument)
	{
		ServerGarlandPipe *sgpPipe = NULL;

		while (pgstaArgument->m_bShouldRun)
		{
			if (sgpPipe == NULL)
			{
				sgpPipe = new ServerGarlandPipe();
			}

			if (sgpPipe->Connect())
			{
				EnterCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				pgstaArgument->m_lClientPipes.push_back(sgpPipe);
				if (pgstaArgument->m_lClientPipes.size() == 1)
				{
					SetEvent(pgstaArgument->m_hNonZeroCountEvent);
				}
				LeaveCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				
				sgpPipe = NULL;
			}
		}

		if (sgpPipe != NULL)
		{
			delete sgpPipe;
		}

		return 0;
	}

	DWORD WINAPI LightingThreadProc(GarlandServerThreadArgument *pgstaArgument)
	{
		MessageGeneratorProvider mgpProvider;
		GarlandMessage gmMessage;

		while (pgstaArgument->m_bShouldRun)
		{
			gmMessage = mgpProvider.GetNext()();
			for (std::list<ServerGarlandPipe *>::iterator it = pgstaArgument->m_lClientPipes.begin(); 
				it != pgstaArgument->m_lClientPipes.end(); ++it)
			{
				if (!(*it)->SendGarlandMessage(&gmMessage))
				{
					(*it)->Disconnect();
				}
			}

			std::list<ServerGarlandPipe *>::iterator it = pgstaArgument->m_lClientPipes.begin(), cur;
			while (it != pgstaArgument->m_lClientPipes.end()) {
				cur = it++;
				if (!(*cur)->IsConnected())
				{
					delete *cur;
					EnterCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
					pgstaArgument->m_lClientPipes.erase(cur);
					LeaveCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				}
			}

			if (pgstaArgument->m_lClientPipes.empty())
			{
				WaitForSingleObject(pgstaArgument->m_hNonZeroCountEvent, INFINITE);
				mgpProvider.Reset();
			}
			else
			{
				Sleep(pgstaArgument->m_dwLightSleepTime);
			}
		}

		return 0;
	}
}

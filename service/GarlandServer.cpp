#include "GarlandServer.h"
#include "MessageGeneratorProvider.h"
#include "..\common\ServerGarlandPipe.h"

namespace NewYearGarlands
{
	GarlandServer::GarlandServer() : m_bIsRunning(FALSE),
		m_hClientListeningThread(INVALID_HANDLE_VALUE), m_hLightingThread(INVALID_HANDLE_VALUE)
	{
		InitializeCriticalSection(&m_csCriticalSection);
	}

	GarlandServer::~GarlandServer()
	{
		Shutdown();
		DeleteCriticalSection(&m_csCriticalSection);
	}

	BOOL GarlandServer::IsRunning()
	{
		return m_bIsRunning;
	}

	BOOL GarlandServer::Start()
	{
		if (!IsRunning())
		{
			EnterCriticalSection(&m_csCriticalSection);
			if (!IsRunning())
			{

			}
			LeaveCriticalSection(&m_csCriticalSection);
		}
	}

	DWORD WINAPI ClientWaitingThreadProc(GarlandServerThreadArgument *pgstaArgument)
	{
		HANDLE hPipe;
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
				pgstaArgument->m_vClientPipes.push_back(sgpPipe);
				if (pgstaArgument->m_vClientPipes.size == 1)
				{
					SetEvent(pgstaArgument->m_hNonZeroCountEvent);
				}
				LeaveCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				
				sgpPipe = NULL;
			}
		}

		SetEvent(pgstaArgument->m_hNonZeroCountEvent);
		if (sgpPipe != NULL)
		{
			delete sgpPipe;
		}
	}

	DWORD WINAPI LightingThreadProc(GarlandServerThreadArgument *pgstaArgument)
	{
		MessageGeneratorProvider mgpProvider;
		GarlandMessage gmMessage;

		while (pgstaArgument->m_bShouldRun)
		{
			gmMessage = mgpProvider.GetNext()();
			for (std::vector<ServerGarlandPipe *>::iterator it = pgstaArgument->m_vClientPipes.begin(); 
				it != pgstaArgument->m_vClientPipes.end(); ++it)
			{
				if (!(*it)->SendGarlandMessage(&gmMessage))
				{
					delete *it;
					EnterCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
					pgstaArgument->m_vClientPipes.erase(it);
					LeaveCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				}
			}

			if (pgstaArgument->m_vClientPipes.empty())
			{
				WaitForSingleObject(pgstaArgument->m_hNonZeroCountEvent, INFINITE);
				mgpProvider.Reset();
			}
		}
	}
}

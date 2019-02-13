#include "GarlandServer.h"
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

		if (sgpPipe != NULL)
		{
			delete sgpPipe;
		}
	}

	DWORD WINAPI LightingThreadProc(GarlandServerThreadArgument *pgstaArgument)
	{
		BYTE bGeneratorsCount = 2, bCurrentGenerator = 0;
		// TODO: add array of function-generators

		while (pgstaArgument->m_bShouldRun)
		{
			if (pgstaArgument->m_vClientPipes.empty())
			{
				WaitForSingleObject(pgstaArgument->m_hNonZeroCountEvent);
			}

			// TODO: get function generator
			for (std::vector<ServerGarlandPipe *>::iterator it = pgstaArgument->m_vClientPipes.begin(); 
				it != pgstaArgument->m_vClientPipes.end(); ++it)
			{
				if (!(*it)->SendGarlandMessage(/* TODO: use generator to generate message */))
				{
					EnterCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
					pgstaArgument->m_vClientPipes.erase(it);
					LeaveCriticalSection(&pgstaArgument->m_csReadWriteCriticalSection);
				}
			}
		}
	}

	const BYTE bColorCount = 3;
	const COLORREF crColors[bColorCount] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };

	GarlandMessage GenerateLightUpMessage()
	{
		GarlandMessage gmMessage;
		gmMessage.crColor = crColors[rand() % bColorCount];
		gmMessage.mtMessageType = LIGHT_UP;
		return gmMessage;
	}

	GarlandMessage GenerateLightOutMessage()
	{
		GarlandMessage gmMessage;
		gmMessage.mtMessageType = LIGHT_OUT;
		return gmMessage;
	}
}

#include "ClientGarlandPipe.h"
#include <exception>

namespace NewYearGarlands
{
	ClientGarlandPipe::ClientGarlandPipe()
		: m_hPipe(INVALID_HANDLE_VALUE)
	{ }

	ClientGarlandPipe::~ClientGarlandPipe()
	{
		if (IsConnected())
		{
			DisconnectFromServer();
		}
	}

	BOOL ClientGarlandPipe::IsConnected()
	{
		return m_hPipe != INVALID_HANDLE_VALUE;
	}

	BOOL ClientGarlandPipe::ConnectToServer()
	{
		return TryConnectToServer(NMPWAIT_WAIT_FOREVER);
	}

	BOOL ClientGarlandPipe::TryConnectToServer(DWORD dwTimeOut)
	{
		if (!IsConnected())
		{
			BOOL bResult = WaitNamedPipe(GetPipeName(), dwTimeOut);
			if (bResult)
			{
				HANDLE hNewPipe = CreateFile(GetPipeName(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
				if (hNewPipe != INVALID_HANDLE_VALUE)
				{
					DWORD dwPipeMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
					bResult = SetNamedPipeHandleState(m_hPipe, &dwPipeMode, NULL, NULL);
					if (bResult)
					{
						m_hPipe = hNewPipe;
					}
					else
					{
						CloseHandle(hNewPipe);
					}
				}
			}
			return IsConnected();
		}
		else
		{
			return FALSE;
		}
	}

	BOOL ClientGarlandPipe::DisconnectFromServer()
	{
		if (m_hPipe != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hPipe);
			m_hPipe = INVALID_HANDLE_VALUE;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL ClientGarlandPipe::ReadGarlandMessage(GarlandMessage* pgmMessage)
	{
		if (IsConnected())
		{
			DWORD dwReadBytes;
			BOOL bReadResult = ReadFile(m_hPipe, pgmMessage, sizeof(GarlandMessage), &dwReadBytes, NULL);

			if (!bReadResult)
			{
				if (GetLastError() == ERROR_MORE_DATA)
				{
					// Connected to illegal server, disconnecting
					DisconnectFromServer();
				}
			}
			return bReadResult;
		}
		else 
		{
			return FALSE;
		}
	}
}

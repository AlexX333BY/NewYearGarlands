#include "ServerGarlandPipe.h"
#include "GarlandMessage.h"
#include <exception>

namespace NewYearGarlands
{
	ServerGarlandPipe::ServerGarlandPipe() : m_bIsConnected(FALSE)
	{ 
		m_hPipe = CreateNamedPipe(GarlandPipe::GetPipeName(), PIPE_ACCESS_OUTBOUND, PIPE_TYPE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
			sizeof(GarlandMessage), 0, 0, NULL);

		if (m_hPipe == INVALID_HANDLE_VALUE)
		{
			throw new std::exception();
		}
	}

	ServerGarlandPipe::~ServerGarlandPipe()
	{
		if (IsConnected())
		{
			Disconnect();
		}
		CloseHandle(m_hPipe);
	}

	BOOL ServerGarlandPipe::Connect()
	{
		if (!IsConnected())
		{
			m_bIsConnected = ConnectNamedPipe(m_hPipe, NULL);
			return m_bIsConnected;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL ServerGarlandPipe::Disconnect()
	{
		if (IsConnected())
		{
			m_bIsConnected = !DisconnectNamedPipe(m_hPipe);
			return m_bIsConnected;
		}
		else
		{
			return FALSE;
		}
	}

	BOOL ServerGarlandPipe::IsConnected()
	{
		return m_bIsConnected;
	}

	BOOL ServerGarlandPipe::SendGarlandMessage(GarlandMessage *pgmMessage)
	{
		if (IsConnected() && (pgmMessage != NULL))
		{
			DWORD dwWrittenBytes;
			return WriteFile(m_hPipe, pgmMessage, sizeof(GarlandMessage), &dwWrittenBytes, NULL) 
				&& (dwWrittenBytes == sizeof(GarlandMessage));
		}
		else
		{
			return FALSE;
		}
	}
}

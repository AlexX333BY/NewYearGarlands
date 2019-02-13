#pragma once

#include "GarlandPipe.h"
#include "GarlandMessage.h"

namespace NewYearGarlands
{
	class ServerGarlandPipe : public GarlandPipe
	{
	public:
		ServerGarlandPipe();
		~ServerGarlandPipe();
		BOOL Connect();
		BOOL Disconnect();
		BOOL IsConnected();
		BOOL SendGarlandMessage(GarlandMessage *pgmMessage);
	protected:
		HANDLE m_hPipe;
		BOOL m_bIsConnected;
	};
}

#pragma once

#include <Windows.h>
#include <vector>
#include "GarlandServerThreadArgument.h"

namespace NewYearGarlands
{
	class GarlandServer
	{
	public:
		GarlandServer();
		GarlandServer(DWORD dwPause);
		~GarlandServer();
		BOOL Start();
		BOOL Shutdown();
		BOOL IsRunning();
	protected:
		volatile BOOL m_bIsRunning;
		HANDLE m_hClientListeningThread;
		HANDLE m_hLightingThread;
		GarlandServerThreadArgument m_gstaArgument;
	};
}

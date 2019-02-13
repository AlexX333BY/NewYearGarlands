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
		~GarlandServer();
		BOOL Start();
		BOOL Shutdown();
		BOOL IsRunning();
	protected:
		volatile BOOL m_bIsRunning;
		HANDLE m_hClientListeningThread;
		HANDLE m_hLightingThread;
		CRITICAL_SECTION m_csCriticalSection;
		GarlandServerThreadArgument m_tpaArgument;
	};
}

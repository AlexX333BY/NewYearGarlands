#pragma once

#include <vector>
#include <Windows.h>
#include "..\common\ServerGarlandPipe.h"

namespace NewYearGarlands
{
	struct GarlandServerThreadArgument
	{
	public:
		GarlandServerThreadArgument();
		~GarlandServerThreadArgument();
		volatile BOOL m_bShouldRun;
		std::vector<ServerGarlandPipe *> m_vClientPipes;
		HANDLE m_hNonZeroCountEvent;
		CRITICAL_SECTION m_csReadWriteCriticalSection;
		DWORD m_dwLightSleepTime;
	};
}

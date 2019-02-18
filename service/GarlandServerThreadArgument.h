#pragma once

#include <list>
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
		std::list<ServerGarlandPipe *> m_lClientPipes;
		HANDLE m_hNonZeroCountEvent;
		CRITICAL_SECTION m_csReadWriteCriticalSection;
		DWORD m_dwLightSleepTime;
	};
}

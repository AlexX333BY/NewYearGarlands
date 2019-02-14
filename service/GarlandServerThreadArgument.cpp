#include "GarlandServerThreadArgument.h"

namespace NewYearGarlands
{
	GarlandServerThreadArgument::GarlandServerThreadArgument()
		: m_bShouldRun(TRUE), m_dwLightSleepTime(1000)
	{
		InitializeCriticalSection(&m_csReadWriteCriticalSection);
		m_hNonZeroCountEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	GarlandServerThreadArgument::~GarlandServerThreadArgument()
	{
		DeleteCriticalSection(&m_csReadWriteCriticalSection);
		CloseHandle(m_hNonZeroCountEvent);
		for (std::vector<ServerGarlandPipe *>::const_iterator it = m_vClientPipes.cbegin(); it != m_vClientPipes.cend(); ++it)
		{
			delete *it;
		}
	}
}

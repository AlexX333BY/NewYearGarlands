#include "LampCloseEvent.h"

namespace NewYearGarlands
{
	LampCloseEvent::LampCloseEvent(int winId, wxEventType wxEventType, const bool bEverConnected)
		: wxEvent(winId, wxEventType), m_bEverConnected(bEverConnected)
	{ }

	wxEvent *LampCloseEvent::Clone() const
	{
		return new LampCloseEvent(*this);
	}

	bool LampCloseEvent::EverConnected()
	{
		return m_bEverConnected;
	}
}

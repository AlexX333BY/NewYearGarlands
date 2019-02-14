#include "GarlandMessageEvent.h"

namespace NewYearGarlands
{
	GarlandMessageEvent::GarlandMessageEvent(int winId, wxEventType wxEventType, const COLORREF crColor)
		: wxEvent(winId, wxEventType), m_crColor(crColor)
	{ }

	wxEvent *GarlandMessageEvent::Clone() const
	{
		return new GarlandMessageEvent(*this);
	}

	COLORREF GarlandMessageEvent::GetColor()
	{
		return m_crColor;
	}

	wxDEFINE_EVENT(LIGHT_UP, GarlandMessageEvent);
	wxDEFINE_EVENT(LIGHT_OUT, GarlandMessageEvent);
}

#include "GarlandMessageEvent.h"

namespace NewYearGarlands
{
	GarlandMessageEvent::GarlandMessageEvent(int winId, wxEventType wxEventType, const wxColour cColor)
		: wxEvent(winId, wxEventType), m_cColor(cColor)
	{ }

	wxEvent *GarlandMessageEvent::Clone() const
	{
		return new GarlandMessageEvent(*this);
	}

	wxColour GarlandMessageEvent::GetColor()
	{
		return m_cColor;
	}

	wxDEFINE_EVENT(LIGHT_UP, GarlandMessageEvent);
	wxDEFINE_EVENT(LIGHT_OUT, GarlandMessageEvent);
}

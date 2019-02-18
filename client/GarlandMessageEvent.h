#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace NewYearGarlands
{
	class GarlandMessageEvent : public wxEvent
	{
	public:
		GarlandMessageEvent(int winId, wxEventType wxEventType, const wxColour cColor);
		wxColour GetColor();
		virtual wxEvent *Clone() const;
	protected:
		const wxColour m_cColor;
	};

	wxDEFINE_EVENT(LIGHT_UP_EVENT, GarlandMessageEvent);
	wxDEFINE_EVENT(LIGHT_OUT_EVENT, GarlandMessageEvent);
}

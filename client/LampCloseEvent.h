#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace NewYearGarlands
{
	class LampCloseEvent : public wxEvent
	{
	public:
		LampCloseEvent(int winId, wxEventType wxEventType, const bool bEverConnected);
		bool EverConnected();
		virtual wxEvent *Clone() const;
	protected:
		const bool m_bEverConnected;
	};

	wxDEFINE_EVENT(LAMP_CLOSE_EVENT, LampCloseEvent);
}

#pragma once

#include "GarlandMessageEvent.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

namespace NewYearGarlands
{
	class GarlandLampFrame : public wxFrame
	{
	public:
		GarlandLampFrame(const wxString& title);
	protected:
		void OnClose(wxCloseEvent& event);
		void OnLightUp(GarlandMessageEvent& event);
		void OnLightOut(GarlandMessageEvent& event);
		HANDLE m_hPipeListeningThread;
	};
}

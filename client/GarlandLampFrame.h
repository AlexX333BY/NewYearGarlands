#pragma once

#include "GarlandMessageEvent.h"
#include "LampCloseEvent.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

namespace NewYearGarlands
{
	class GarlandLampFrame : public wxFrame
	{
	public:
		GarlandLampFrame(const wxString& title, byte bPercentIndent = 10);
	protected:
		void OnClose(LampCloseEvent& event);
		void OnLightUp(GarlandMessageEvent& event);
		void OnLightOut(GarlandMessageEvent& event);
		void OnPaint(wxPaintEvent& event);
		const byte m_bPercentIndent;
		wxColour m_currentLampColour;
		HANDLE m_hPipeListeningThread;
	};
}

#pragma once

#include "..\common\ClientGarlandPipe.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

namespace NewYearGarlands
{
	class GarlandLampFrame : public wxFrame
	{
	public:
		GarlandLampFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	protected:
		void OnExit(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();
		ClientGarlandPipe m_cgpPipe;
	};
}

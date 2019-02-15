#include "..\common\ClientGarlandPipe.h"
#include "GarlandLampFrame.h"

namespace NewYearGarlands
{
	const wxString noServerMessage("No server is running now");
	const wxString disconnectedMessage("Server disconnected");

	VOID SendMessageEvent(GarlandLampFrame *glfFrame, GarlandMessage *gmMessage)
	{
		GarlandMessageEvent gmeEvent(glfFrame->GetId(), 
			gmMessage->mtMessageType == LIGHT_UP ? LIGHT_UP_EVENT : LIGHT_OUT_EVENT, 
			gmMessage->crColor);
		gmeEvent.SetEventObject(glfFrame);
		glfFrame->ProcessWindowEvent(gmeEvent);
	}

	VOID SendCloseEvent(GarlandLampFrame *glfFrame, bool bEverConnected)
	{
		LampCloseEvent lceEvent(glfFrame->GetId(), LAMP_CLOSE_EVENT, bEverConnected);
		lceEvent.SetEventObject(glfFrame);
		glfFrame->ProcessWindowEvent(lceEvent);
	}

	DWORD WINAPI PipeListeningThread(GarlandLampFrame *glfFrame)
	{
		ClientGarlandPipe cgpPipe;
		GarlandMessage gmMessage;
		
		bool bEverConnected = cgpPipe.ConnectToServer();
		while (cgpPipe.ReadGarlandMessage(&gmMessage))
		{
			SendMessageEvent(glfFrame, &gmMessage);
		}

		SendCloseEvent(glfFrame, bEverConnected);
		return 0;
	}

	GarlandLampFrame::GarlandLampFrame(const wxString& title, byte bPercentIndent) : 
		wxFrame(NULL, -1, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE),
		m_bPercentIndent(bPercentIndent), m_currentLampColour(GetBackgroundColour())
	{
		Bind(LAMP_CLOSE_EVENT, &GarlandLampFrame::OnClose, this);
		Bind(LIGHT_UP_EVENT, &GarlandLampFrame::OnLightUp, this);
		Bind(LIGHT_OUT_EVENT, &GarlandLampFrame::OnLightOut, this);
		Bind(wxEVT_PAINT, &GarlandLampFrame::OnPaint, this);
		m_hPipeListeningThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PipeListeningThread, this, 0, NULL);
	}

	void GarlandLampFrame::OnClose(LampCloseEvent& event)
	{
		CloseHandle(m_hPipeListeningThread);
		if (event.EverConnected())
		{
			wxMessageBox(disconnectedMessage);
		}
		else
		{
			wxMessageBox(noServerMessage);
		}
		Close(true);
	}

	void GarlandLampFrame::OnLightOut(GarlandMessageEvent& event)
	{
		wxColour colour(GetBackgroundColour());
		m_currentLampColour = colour;
		Refresh();
	}

	void GarlandLampFrame::OnLightUp(GarlandMessageEvent& event)
	{
		wxColour colour(event.GetColor());
		m_currentLampColour = colour;
		Refresh();
	}

	void GarlandLampFrame::OnPaint(wxPaintEvent& event)
	{
		wxSize windowSize = GetClientSize();
		wxPoint centerPoint(windowSize.GetWidth() / 2, windowSize.GetHeight() / 2);
		wxPaintDC wxDc(this);
		wxBrush brush(m_currentLampColour), oldBrush = wxDc.GetBrush();
		wxDc.SetBrush(brush);
		wxDc.DrawCircle(centerPoint, min(windowSize.GetHeight(), windowSize.GetWidth()) * (100 - 2 * m_bPercentIndent) / 100 / 2);
		wxDc.SetBrush(oldBrush);
		event.Skip();
	}
}

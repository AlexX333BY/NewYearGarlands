#include "..\common\ClientGarlandPipe.h"
#include "GarlandLampFrame.h"

namespace NewYearGarlands
{
	VOID SendMessageEvent(GarlandLampFrame *glfFrame, GarlandMessage *gmMessage)
	{
		GarlandMessageEvent gmeEvent(glfFrame->GetId(), 
			gmMessage->mtMessageType == LIGHT_UP ? LIGHT_UP_EVENT : LIGHT_OUT_EVENT, 
			gmMessage->crColor);
		gmeEvent.SetEventObject(glfFrame);
		glfFrame->ProcessWindowEvent(gmeEvent);
	}

	DWORD WINAPI PipeListeningThread(GarlandLampFrame *glfFrame)
	{
		ClientGarlandPipe cgpPipe;
		GarlandMessage gmMessage;

		cgpPipe.ConnectToServer();
		while (cgpPipe.ReadGarlandMessage(&gmMessage))
		{
			SendMessageEvent(glfFrame, &gmMessage);
		}

		glfFrame->Close(true);
		return 0;
	}

	GarlandLampFrame::GarlandLampFrame(const wxString& title) : wxFrame(NULL, -1, title)
	{
		m_hPipeListeningThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PipeListeningThread, this, 0, NULL);
		Bind(wxEVT_CLOSE_WINDOW, &GarlandLampFrame::OnClose, this, GetId());
		Bind(LIGHT_UP_EVENT, &GarlandLampFrame::OnLightUp, this, GetId());
		Bind(LIGHT_OUT_EVENT, &GarlandLampFrame::OnLightOut, this);
	}

	void GarlandLampFrame::OnClose(wxCloseEvent& event)
	{
		CloseHandle(m_hPipeListeningThread);
		event.Skip();
	}

	void GarlandLampFrame::OnLightOut(GarlandMessageEvent& event)
	{

	}

	void GarlandLampFrame::OnLightUp(GarlandMessageEvent& event)
	{

	}
}

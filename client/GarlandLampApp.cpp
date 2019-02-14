#include "GarlandLampApp.h"
#include "GarlandLampFrame.h"

namespace NewYearGarlands
{
	bool GarlandLampApp::OnInit()
	{
		GarlandLampFrame *glfFrame = new GarlandLampFrame("Garland Lamp");
		glfFrame->Show(true);
		return true;
	}
}

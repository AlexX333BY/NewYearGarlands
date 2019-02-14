#pragma once

#include <Windows.h>

namespace NewYearGarlands
{
	enum GarlandMessageType
	{
		LIGHT_UP,
		LIGHT_OUT
	};

	struct GarlandMessage
	{
		COLORREF crColor;
		GarlandMessageType mtMessageType;
	};
}

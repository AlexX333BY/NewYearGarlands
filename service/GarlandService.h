#pragma once

#include <Windows.h>

namespace NewYearGarlands
{
	const LPCTSTR lpcsServiceName = "GarlandService";
	const LPCTSTR lpcsServiceDescription = "Garland lamps service";

	BOOL StartGarlandService();
}

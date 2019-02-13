#include "GarlandPipe.h"

namespace NewYearGarlands
{
	const LPCSTR lpsPipeName = "\\.\\pipe\\LOCAL";

	LPCSTR GarlandPipe::GetPipeName()
	{
		return lpsPipeName;
	}
}

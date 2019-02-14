#pragma once

#include "..\common\GarlandMessage.h"
#include "vector"

namespace NewYearGarlands
{
	typedef GarlandMessage(*MessageGenerator)();

	class MessageGeneratorProvider
	{
	public:
		MessageGeneratorProvider();
		VOID Reset();
		MessageGenerator GetNext();
	protected:
		BYTE m_bCurrentGenerator;
		std::vector<MessageGenerator> m_vGenerators;
	};
}

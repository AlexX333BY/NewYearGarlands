#include "MessageGeneratorProvider.h"

namespace NewYearGarlands
{
	const BYTE bColorCount = 3;
	const COLORREF crColors[bColorCount] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255) };

	GarlandMessage GenerateLightUpMessage()
	{
		GarlandMessage gmMessage;
		gmMessage.crColor = crColors[rand() % bColorCount];
		gmMessage.mtMessageType = LIGHT_UP;
		return gmMessage;
	}

	GarlandMessage GenerateLightOutMessage()
	{
		GarlandMessage gmMessage;
		gmMessage.mtMessageType = LIGHT_OUT;
		return gmMessage;
	}

	MessageGeneratorProvider::MessageGeneratorProvider() : m_bCurrentGenerator(0)
	{
		m_vGenerators.push_back(GenerateLightUpMessage);
		m_vGenerators.push_back(GenerateLightOutMessage);
	}

	VOID MessageGeneratorProvider::Reset()
	{
		m_bCurrentGenerator = 0;
	}

	MessageGenerator MessageGeneratorProvider::GetNext()
	{
		m_bCurrentGenerator = ++m_bCurrentGenerator % m_vGenerators.size();
		return m_vGenerators[m_bCurrentGenerator];
	}
}

#include "GarlandPipe.h"
#include "GarlandMessage.h"

namespace NewYearGarlands
{
	class ClientGarlandPipe : public GarlandPipe
	{
	public:
		ClientGarlandPipe();
		~ClientGarlandPipe();
		BOOL ConnectToServer();
		BOOL TryConnectToServer(DWORD dwTimeOut);
		BOOL DisconnectFromServer();
		BOOL ReadGarlandMessage(GarlandMessage* pgmMessage);
		BOOL IsConnected();
	protected:
		HANDLE m_hPipe;
	};
}

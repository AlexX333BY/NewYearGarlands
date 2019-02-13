#include "GarlandPipe.h"
#include "GarlandMessage.h"

namespace NewYearGarlands
{
	class ClientGarlandPipe : GarlandPipe
	{
	public:
		ClientGarlandPipe();
		~ClientGarlandPipe();
		BOOL ConnectToServer();
		BOOL TryConnectToServer(DWORD dwTimeOut);
		BOOL DisconnectFromServer();
		BOOL ReadGarlandMessage(GarlandMessage* pgmMessage);
		BOOL IsConnected();
	private:
		HANDLE m_hPipe;
	};
}

#include "HNetwork.h"

void main()
{
	HANDLE hExcuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALIAS_EXISTS)
	{
		CloseHandle(hExcuteMutex);
		return;
	}
	HNetwork net;
	if (net.InitNetwork("192.168.0.154", 10000) == false)
	{
		return;
	}
	net.Run();
	net.DeleteNetwork();
	CloseHandle(hExcuteMutex);
}
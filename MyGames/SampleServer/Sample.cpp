#include "HLoginServer.h"


void main()
{
	HLoginServer server;
	server.Init();
	server.Run();
	server.Release();
}
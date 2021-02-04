#include "HNetUser.h"

HNetUser::HNetUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * 10000);
}

HNetUser::~HNetUser()
{

}


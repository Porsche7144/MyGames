#include "HChatUser.h"
#include "HSessionMgr.h"

//	 |p1------p1e:p2-------p2e:p3-------t----p3e-------|
//	 1, ��Ŷ ��� > dwTrans
//	 2, ��Ŷ ��� == dwTrans
//	 3, ��Ŷ ��� < dwTrans
		// 3.1, ��Ŷ ũ�� == dwTrans
		// 3.2, ��Ŷ ũ�� > dwTrans
		// 3.3, ��Ŷ ũ�� < dwTrans

//	0    ~    10000
//	  50byte
//	 --------(s)9985,9990   10;
//	 �ӽù��� �޴� �ܿ����� ����
//	clear   0    ~    10000
//	 �� ���ۿ� �ӽù����� �ܿ��� ������ ����
//	clear   0~5: 50byte   ~    10000

HChatUser::HChatUser()
{
}

HChatUser::~HChatUser()
{
}

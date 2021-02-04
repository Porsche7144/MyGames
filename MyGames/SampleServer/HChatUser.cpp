#include "HChatUser.h"
#include "HSessionMgr.h"

//	 |p1------p1e:p2-------p2e:p3-------t----p3e-------|
//	 1, 패킷 헤더 > dwTrans
//	 2, 패킷 헤더 == dwTrans
//	 3, 패킷 헤더 < dwTrans
		// 3.1, 패킷 크기 == dwTrans
		// 3.2, 패킷 크기 > dwTrans
		// 3.3, 패킷 크기 < dwTrans

//	0    ~    10000
//	  50byte
//	 --------(s)9985,9990   10;
//	 임시버퍼 받는 잔여량을 저장
//	clear   0    ~    10000
//	 주 버퍼에 임시버퍼의 잔여량 데이터 복사
//	clear   0~5: 50byte   ~    10000

HChatUser::HChatUser()
{
}

HChatUser::~HChatUser()
{
}

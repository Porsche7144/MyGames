#pragma once
#pragma once
#include <windows.h>
#include <iostream>


//		typedef signed char					 int8_t;
//		typedef short						 int16_t;
//		typedef int							 int32_t;
//		typedef long long					 int64_t;
//		typedef unsigned char				 uint8_t;
//		typedef unsigned short				 uint16_t;
//		typedef unsigned int				 uint32_t;
//		typedef unsigned long long			 uint64_t;
//											
//		typedef signed char					 int_least8_t;
//		typedef short						 int_least16_t;
//		typedef int							 int_least32_t;
//		typedef long long					 int_least64_t;
//		typedef unsigned char				 uint_least8_t;
//		typedef unsigned short				 uint_least16_t;
//		typedef unsigned int				 uint_least32_t;
//		typedef unsigned long long			 uint_least64_t;
//											
//		typedef signed char					 int_fast8_t;
//		typedef int							 int_fast16_t;
//		typedef int							 int_fast32_t;
//		typedef long long					 int_fast64_t;
//		typedef unsigned char				 uint_fast8_t;
//		typedef unsigned int				 uint_fast16_t;
//		typedef unsigned int				 uint_fast32_t;
//		typedef unsigned long long			 uint_fast64_t;
//											
//		typedef long long					 intmax_t;
//		typedef unsigned long long			 uintmax_t;


#pragma pack(push, 1)
typedef struct
{
	uint16_t iotype;  // 패킷 입출력 종류
	uint16_t len;	  // MSG 바이트 크기
	uint16_t type;	  // 패킷 타입
	uint16_t time;	  // 패킷 전송시간
}PACKET_HEADER;

#define PACKET_HEADER_SIZE 10

typedef struct
{
	PACKET_HEADER ph;
	int8_t		  msg[1024];
}UPACKET, *P_UPACKET;

struct HChatMsg
{
	int  iCnt;
	char buffer[128];
};


typedef struct
{
	uint16_t	Point;				// 미분배 포인트.
	uint16_t	STR;				// 힘.
	uint16_t	INT;				// 지능.
	uint16_t	DEX;				// 민첩.
	uint16_t	CHA;				// 카리스마.
	uint16_t	Life;				// 현재 생명력. (힘 종속)
	uint16_t	Stamina;			// 현재 지구력. (힘 종속)
	int8_t		Status;				// 현재 상태 값.
	uint16_t	Skins[6];			// 현재 모습 상태.(0:머리, 1:얼굴)
	int8_t		Parts[4];			// 부위별 피해 상황. 
	int8_t		Level;				// 레벨.
	uint32_t	Exp;				// 경험치.
	uint32_t	Skills[16];			// 스킬 숙련도. (0->10,000)

}HSTATUS_VARS;

//------------------------------------------------------------------------------

typedef struct
{
	int8_t		Rank;				// 계급.
	int8_t		Medals;				// 획득한 훈장 수.
	uint16_t	Fame;				// 명성 수치.
	uint32_t	Win;				// 승.
	uint32_t	Lose;				// 패.

}HSCORE_VARS;
#pragma pack(pop)


enum HPACKET_TYPE
{
	PACKET_USERNAME_REQ = 10,
	PACKET_USERNAME_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_HSTATUS_VARS,
};
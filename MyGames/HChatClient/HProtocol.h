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
	uint16_t iotype;  // ��Ŷ ����� ����
	uint16_t len;	  // MSG ����Ʈ ũ��
	uint16_t type;	  // ��Ŷ Ÿ��
	uint16_t time;	  // ��Ŷ ���۽ð�
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
	uint16_t	Point;				// �̺й� ����Ʈ.
	uint16_t	STR;				// ��.
	uint16_t	INT;				// ����.
	uint16_t	DEX;				// ��ø.
	uint16_t	CHA;				// ī������.
	uint16_t	Life;				// ���� �����. (�� ����)
	uint16_t	Stamina;			// ���� ������. (�� ����)
	int8_t		Status;				// ���� ���� ��.
	uint16_t	Skins[6];			// ���� ��� ����.(0:�Ӹ�, 1:��)
	int8_t		Parts[4];			// ������ ���� ��Ȳ. 
	int8_t		Level;				// ����.
	uint32_t	Exp;				// ����ġ.
	uint32_t	Skills[16];			// ��ų ���õ�. (0->10,000)

}HSTATUS_VARS;

//------------------------------------------------------------------------------

typedef struct
{
	int8_t		Rank;				// ���.
	int8_t		Medals;				// ȹ���� ���� ��.
	uint16_t	Fame;				// �� ��ġ.
	uint32_t	Win;				// ��.
	uint32_t	Lose;				// ��.

}HSCORE_VARS;
#pragma pack(pop)


enum HPACKET_TYPE
{
	PACKET_USERNAME_REQ = 10,
	PACKET_USERNAME_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_HSTATUS_VARS,
};
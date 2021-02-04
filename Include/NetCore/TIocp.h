
#pragma once
#include "TNetStd.h"
#include "TNetUser.h"
#define MAX_WORKER_THREAD 3
class TServer;

class TIocp 
{
public:
	/// TODO : �����ν��Ͻ�
	TServer* m_pServer;///< TServer point>	
	HANDLE   m_hIOCP;
	HANDLE   m_hWorkThread[MAX_WORKER_THREAD];
	DWORD    m_dwThreadID[MAX_WORKER_THREAD];
public:
	/**
		* �ʱ�ȭ
		*
		* @return bool
	*/
	bool	Init();
	bool	Run();
	bool	Release();
/// <A short one line description>
///
/// <Longer description>
/// <May span multiple lines or paragraphs as needed>
///
/// @param  Ŭ���̾�Ʈ ����
/// @param  ��ȯŰ
/// @return Description of the return value
	void    SetBind(SOCKET sock, ULONG_PTR key);
public:
	/**
		* ������
		* @param TServer*
		*/
	TIocp(TServer*);
	virtual ~TIocp();
};


#include "HCore.h"

bool HCore::GameInit() 
{
	m_bGameRun = true;
	g_Timer.Init();
	g_Input.Init();
	g_Write.Init();
	Init();
	return true;
}
bool HCore::GameRelease()
{	
	Release();
	g_Timer.Release();
	g_Input.Release();
	g_Write.Release();

	return true;
}
bool	HCore::GameFrame()
{
	PreFrame();
	g_Timer.Frame();	
	g_Input.Frame();
		Frame();
	g_Write.Frame();
	PostFrame();
	return true;
}
bool	HCore::PreRender()
{
	return true;
}
bool	HCore::PostRender()
{
	g_Timer.Render();
	g_Input.Render();
	g_Write.Render();	
	g_Write.Draw(0, 0, g_Timer.m_szBuffer);
	return true;
}
bool	HCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}
bool	HCore::GameRun()
{
	if (GameFrame()==false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool HCore::Run()
{
	//HeapEnableTerminateOnCorruption �ɼ��� �����ϸ� �ջ�� ���� �̿��ϴ� ���� �ǿ뿡 ���� ���ø����̼��� ������ ���� �� �����Ƿ� �����ϰ� ����.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx �� COM ���̺귯���� ����ϴ� �� �����忡 ���� �� �� �̻� ȣ���ؾ�  �Ѵ�.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			if (GameRun()==false)
			{
				break;
			}
		}
	}
	GameRelease();
	CoUninitialize();
	return true;
}
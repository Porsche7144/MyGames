#include "Sample.h"

LRESULT	 Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pMainCamera == nullptr) return -1;
	m_pMainCamera->WndProc(hWnd, message, wParam, lParam);

	return -1;
}

#pragma region Init
bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,300,-100 }, { 0,0,0 });
	
	m_pObj.Load(g_pd3dDevice, "../../Image/FBX_Image/man.fbx");
	//m_pObj.m_pMainCamera = m_pMainCamera;


#pragma region Camera Init
	// Ä«¸Þ¶ó
	m_ModelCamera.CreateViewMatrix({ 0,30,-30 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_ModelCamera.CreateProjectionMatrix(1, 10000, HBASIS_PI / 4.0f, fAspect);
	m_ModelCamera.Init();
#pragma endregion

	HCore::m_bFrameRun = true;

	return true;
}
#pragma endregion

#pragma region Frame
bool Sample::Frame()
{


#pragma region UserInput
/*
	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(1.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_UserShape.FrontMovement(-1.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_UserShape.RightMovement(-1.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_UserShape.RightMovement(1.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_UserShape.UpMovement(1.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_UserShape.UpMovement(-1.0f);
	}*/
#pragma endregion

	return true;
}
#pragma endregion

#pragma region Render
bool Sample::Render()
{

#pragma region ModelRender
	//for (auto data : m_ModelMatrixList)
	//{
	//	m_pObj.m_pFbxObj->SetMatrix(&data, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	//	m_pObj.Render(g_pImmediateContext);
	//}
#pragma endregion

	if (g_Input.GetKey('W') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(20.0f);
	}
	if (g_Input.GetKey('S') == KEY_HOLD)
	{
		m_pMainCamera->FrontMovement(-20.0f);
	}
	if (g_Input.GetKey('A') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(-20.0f);
	}
	if (g_Input.GetKey('D') == KEY_HOLD)
	{
		m_pMainCamera->RightMovement(20.0f);
	}
	if (g_Input.GetKey('Q') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(20.0f);
	}
	if (g_Input.GetKey('E') == KEY_HOLD)
	{
		m_pMainCamera->UpMovement(-20.0f);
	}
#pragma endregion


	return true;
}
#pragma endregion

#pragma region PostRender
bool Sample::PostRender()
{
	HCore::PostRender();
	return true;
}
#pragma endregion

#pragma region Release
bool Sample::Release()
{

	return true;
}
#pragma endregion


#include "HCore.h"
#include "HTextureManager.h"

bool HCore::m_bFrameRun = false;

HRESULT HCore::DeleteDXResource()
{
	g_dxWrite.DeleteDependentResource();
	return S_OK;
}
HRESULT HCore::CreateDXResource(UINT w, UINT h)
{
	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	g_dxWrite.ResizeDevice(w, h, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();
	return S_OK;
}
bool	HCore::PreInit()
{
	return true;
};
bool	HCore::Init()
{
	return true;
};
bool	HCore::PostInit()
{

	return true;
}
bool HCore::CameraFrame()
{

	if (g_Input.GetKey('1') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('2') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('3') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_BACK;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	if (g_Input.GetKey('4') == KEY_PUSH)
	{
		HDxState::g_RasterizerDesc.CullMode = D3D11_CULL_FRONT;
		HDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext, HDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, HDxState::g_pRSEdit.Get());
	}

	//if (g_Input.GetKey('W') == KEY_HOLD)
	//{
	//	m_pMainCamera->FrontMovement(20.0f);
	//}
	//if (g_Input.GetKey('S') == KEY_HOLD)
	//{
	//	m_pMainCamera->FrontMovement(-20.0f);
	//}
	//if (g_Input.GetKey('A') == KEY_HOLD)
	//{
	//	m_pMainCamera->RightMovement(-20.0f);
	//}
	//if (g_Input.GetKey('D') == KEY_HOLD)
	//{
	//	m_pMainCamera->RightMovement(20.0f);
	//}
	//if (g_Input.GetKey('Q') == KEY_HOLD)
	//{
	//	m_pMainCamera->UpMovement(20.0f);
	//}
	//if (g_Input.GetKey('E') == KEY_HOLD)
	//{
	//	m_pMainCamera->UpMovement(-20.0f);
	//}

	m_pMainCamera->Frame();

	return true;
}
;
bool HCore::GameInit()
{
	PreInit();
	m_bGameRun = true;
	if (HDevice::Init() == false)
	{
		return false;
	}
	SetMode(m_bFullScreen);

	g_Timer.Init();
	g_Input.Init();
	//g_SoundMgr.Init();

	IDXGISurface1* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)&pBackBuffer);
	g_dxWrite.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer);
	if (pBackBuffer) pBackBuffer->Release();

	m_Camera.CreateViewMatrix({ 0, 10, -10 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjectionMatrix(1, 100000, HBASIS_PI / 4.0f, fAspect);
	m_Camera.Init();
	m_pMainCamera = &m_Camera;

	if (!m_SkyBox.Create(g_pd3dDevice, L"../../data/Shader/SkyObjectVS.txt", L"../../data/Shader/SkyObjectPS.txt", L""))
	{
		return false;
	}

	if (!m_LineShape.Create(g_pd3dDevice, L"../../data/Shader/LineVS.txt", L"../../data/Shader/LinePS.txt", L"../../Image/tileA.jpg"))
	{
		return false;
	}

	g_TextureMgr.Init();

	//Init();
	//m_bFrameRun = true;
	PostInit();

	return true;
}

bool HCore::InitTool(HWND hwnd, HINSTANCE hInstance)
{
	HRESULT hr;
	g_hWnd = m_hWnd = hwnd;
	g_hInstance = m_hInstance = hInstance;

	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	GetWindowRect(m_hWnd, &m_rtWindow);

	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();

	return true;
}

bool HCore::ReleaseTool()
{
	GameRelease();
	CoUninitialize();

	return true;
}

bool HCore::GameRelease()
{
	m_LineShape.Release();
	m_SkyBox.Release();
	Release();
	g_Timer.Release();
	g_Input.Release();
	//g_SoundMgr.Release();
	//g_ObjectMgr.Release();
	g_dxWrite.Release();
	HDevice::Release();
	return true;
}
bool	HCore::GameFrame()
{

	PreFrame();
	g_Timer.Frame();
	g_Input.Frame();
	// g_SoundMgr.Frame();
	m_SkyBox.Frame();
	//Frame();
	// g_ObjectMgr.Frame();
	CameraFrame();
	PostFrame();
	return true;
}

bool HCore::PostFrame()
{
	if (m_bFrameRun)
	{
		Frame();
	}
	return true;
}

bool	HCore::PreRender()
{
	HDevice::PreRender();

	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_SkyBox.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_SkyBox.Render(g_pImmediateContext);

	g_pImmediateContext->RSSetState(HDxState::g_pRSBackCullSolid);
	g_pImmediateContext->PSSetSamplers(0, 1, &HDxState::g_pSSWrapLinear);
	g_pImmediateContext->OMSetDepthStencilState(HDxState::g_pDSSDepthEnable, 0);
	return true;
}
bool	HCore::PostRender()
{
	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_LineShape.Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(100, 0, 0), Vector4(1, 0, 0, 1));
	m_LineShape.Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(0, 100, 0), Vector4(0, 1, 0, 1));
	m_LineShape.Draw(g_pImmediateContext, Vector3(0, 0, 0), Vector3(0, 0, 100), Vector4(0, 0, 1, 1));

	g_Timer.Render();
	g_Input.Render();
	// g_SoundMgr.Render();

	g_dxWrite.Render();
	g_dxWrite.Draw(0, 0, g_Timer.m_szBuffer);
	HDevice::PostRender();

	return true;
}
bool	HCore::GameRender()
{
	if (PreRender() == false) return false;
	if (m_bFrameRun)
	{
		if (Render() == false) return false;
	}
	if (PostRender() == false) return false;
	return true;
}
bool	HCore::GameRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
bool HCore::Run()
{
	//HeapEnableTerminateOnCorruption 옵션을 설정하면 손상된 힙을 이용하는 보안 악용에 대한 애플리케이션의 노출을 줄일 수 있으므로 강력하게 권장.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (GameInit())
	{
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
	}
	while (m_bGameRun)
	{
		if (MsgProcess() == false)
		{
			m_bGameRun = false;
		}
		else
		{
			if (GameRun() == false)
			{
				break;
			}
		}
	}
	GameRelease();
	CoUninitialize();
	return true;
}

bool HCore::ToolRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;

	return true;
}

HCore::HCore()
{
	m_pMainCamera = nullptr;
}

HCore::~HCore()
{
}

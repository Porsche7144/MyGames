#pragma once
#include "HBasisStd.h"
#include <d3d11.h>
#include <wrl.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#if NDEBUG
#pragma comment (lib, "Basis3DLib_R.lib")
#else
#pragma comment (lib, "Basis3DLib_D.lib")
#endif

using namespace Microsoft::WRL;
using namespace std;

namespace HBASIS_CORE_LIB
{
	struct HKeyMap
	{
		bool   m_bFront; // up, w
		bool   m_bLeft;  // left, a
		bool   m_bRight; // right. d
		bool   m_bBack;  // down, s
		bool   m_bAttack;// enter, lbutton
		bool   m_bJump;  // space, rbutton
		bool   m_bHangul;
		bool   m_bCapsLock;
		bool   m_bNumLock;
	};

	extern ID3D11Device* g_pd3dDevice;
	extern ID3D11DeviceContext* g_pImmediateContext;
	extern HKeyMap  g_KeyMap;
	extern float	g_fGameTimer;
	extern float    g_fSecondPerFrame;
	extern HWND     g_hWnd;
	extern HINSTANCE     g_hInstance;
	extern RECT		g_rtClient;
	extern bool		g_bActive;
}

using namespace HBASIS_CORE_LIB;
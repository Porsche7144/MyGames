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


/////////////////////////////////////////  ��ȯ���� �ʴ´�. ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H
//���ǻ���:��ũ�� �Լ��ȿ��� ��ũ�� �Լ�(x�� �Լ��� �ؼ�)�� �ߺ� ���� ���� �ʰ� x=DX�޼ҵ常 ����ؾ� �Ѵ�.
#define H(x){ hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__), __LINE__);\
							MessageBox(NULL, buffer,output,MB_OK);}\
						}
#endif
#else
#ifndef H
#define H(x) (x)
#endif
#endif

/////////////////////////////////////////  ������ ��츸 ��ȯ�Ѵ� ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef H_RETURN
//���ǻ���:hr = (x); �� �־�� ��ũ�� �Լ��ȿ��� ��ũ�� �Լ�(x�� �Լ��� �ؼ�)�� �ߺ� �������� �ʴ´�.
#define H_RETURN(x){  hr = (x); if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
						}
#endif
#else
#define H_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; }}
#endif

/////////////////////////////////////////  ������ ��ȯ�Ѵ� ////////////////////////////////////////////////////////
#if defined(DEBUG) | defined(_DEBUG) 
#ifndef U_RETURN
//���ǻ���:hr = (x); �� �־�� ��ũ�� �Լ��ȿ��� ��ũ�� �Լ�(x�� �Լ��� �ؼ�)�� �ߺ� �������� �ʴ´�.
#define U_RETURN(x){  hr = (x);if (FAILED(hr)){\
							LPWSTR output;\
							WCHAR buffer[256]={0,};\
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS |FORMAT_MESSAGE_ALLOCATE_BUFFER,\
							NULL,x,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&output,0,NULL);\
							wsprintf(buffer,L"File=%s\nLine=%d", _T(__FILE__),__LINE__);\
							MessageBox(NULL, buffer,output,MB_OK); return hr;}\
							else { return hr;}\
						}
#endif
#else
#define U_RETURN(x){ hr = (x); return hr; }
#endif


#ifndef V
#define V(x)       { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#ifndef T_RETURN
#define T_RETURN(x,ret){ hr = (x); if( FAILED(hr) ) { return ret; } }
#endif


//////////////////////////////////////////////
// ��ü �� �迭 �Ҵ�� ���� �� �Ҹ� ��ũ��
//////////////////////////////////////////////

#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

using namespace HBASIS_CORE_LIB;
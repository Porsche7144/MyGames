#include "HWrite.h"
#include "HTimer.h"


////////////////////////////////////////////////////////
HRESULT HDxWrite::CreateIndependentResource()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_pd2dFactory);
	if (FAILED(hr))
	{
		return hr;
	}
	m_pd2dFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);

	IDWriteFactory*		m_pd2dWrite;
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_pd2dWrite);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pd2dWrite->CreateTextFormat(
		L"굴림",//L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30,
		L"ko-kr",//L"en-us",//L"ko-kr"
		&m_pTextFormat);
	if (FAILED(hr))
	{
		return hr;
	}


	return S_OK;
}
HRESULT HDxWrite::CreateDependentResource(
	int iWidth, int iHeight,
	IDXGISurface1* pSurface)
{
	if (m_pd2dFactory == nullptr) return S_OK;
	D2D1_RENDER_TARGET_PROPERTIES Properties;
	Properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	Properties.pixelFormat.alphaMode =
		D2D1_ALPHA_MODE_PREMULTIPLIED;
	Properties.dpiX = m_fdpiX;
	Properties.dpiY = m_fdpiY;
	Properties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Properties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
		pSurface,
		&Properties,
		&m_pd2dRT);
	if (FAILED(hr))
	{
		return hr;
	}
	D2D1_COLOR_F color;
	color.r = 255.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 255.0f;

	hr = m_pd2dRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pBrush);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
void HDxWrite::DeleteDependentResource()
{
	if (m_pBrush)
	{
		m_pBrush->Release();
	}
	if (m_pd2dRT)
	{
		m_pd2dRT->Release();
	}
	m_pBrush = nullptr;
	m_pd2dRT = nullptr;
}
bool HDxWrite::Set(HWND hWnd, int iWidth, int iHeight,
	IDXGISurface1* pSurface)
{
	// 디바이스 종속성, 디바이스 독립적
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	CreateIndependentResource();
	CreateDependentResource(iWidth, iHeight, pSurface);
	return false;
}
void HDxWrite::ResizeDevice(
	int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	CreateDependentResource(iWidth, iHeight, pSurface);
}
bool HDxWrite::Init()
{
	return false;
}

bool HDxWrite::Frame()
{
	return false;
}

bool HDxWrite::Render()
{
	if (m_pd2dRT == nullptr) return true;
	m_pd2dRT->BeginDraw();
	{
		std::lock_guard<std::mutex> lock(m_hMutex);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			m_TextList[iMsg].rt.top = 25 + iMsg * 25;
			D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
			m_pd2dRT->SetTransform(matWorld);
			D2D1_RECT_F layoutRect =
			{
				m_TextList[iMsg].rt.left,
				m_TextList[iMsg].rt.top,
				m_TextList[iMsg].rt.right,
				m_TextList[iMsg].rt.bottom,
			};
			D2D1::ColorF fColor = m_TextList[iMsg].dwColor;
			m_pBrush->SetColor(fColor);
			m_pd2dRT->DrawText(
				m_TextList[iMsg].msg.c_str(),
				m_TextList[iMsg].msg.size(),
				m_pTextFormat,
				&layoutRect,
				m_pBrush);
		}
	}
	m_pd2dRT->EndDraw();
	return false;
}

bool HDxWrite::Release()
{
	if (m_pd2dFactory == nullptr) return true;
	if (m_pBrush)m_pBrush->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pd2dWrite)m_pd2dWrite->Release();
	if (m_pTextFormat)m_pTextFormat->Release();
	return false;
}
void   HDxWrite::push(HMsg& msg)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(m_TextList.begin(), msg);
}
void HDxWrite::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x, y };
	Draw(p, msg, color);
}
void HDxWrite::Draw(POINT pos, wstring msg, DWORD color)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_pd2dRT == nullptr) return;
	m_pd2dRT->BeginDraw();
	{
		D2D1_RECT_F layoutRect =
		{
			pos.x,pos.y, g_rtClient.right, g_rtClient.bottom
		};
		D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
		m_pd2dRT->SetTransform(matWorld);
		D2D1::ColorF fColor = color;
		m_pBrush->SetColor(fColor);
		m_pd2dRT->DrawText(
			msg.c_str(),
			msg.size(),
			m_pTextFormat,
			&layoutRect,
			m_pBrush);
	}
	m_pd2dRT->EndDraw();
}

HDxWrite::HDxWrite()
{
	m_pd2dFactory = nullptr;
	m_pd2dRT = nullptr;
	m_pd2dWrite = nullptr;
	m_pTextFormat = nullptr;
}

HDxWrite::~HDxWrite()
{
	//Release();
}

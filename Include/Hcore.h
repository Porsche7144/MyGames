#pragma once
#include "HDevice.h"
#include "HTimer.h"
#include "HInput.h"
#include "HWrite.h"
#include "HDebugCamera.h"
#include "HSky.h"

class HCore : public HDevice
{
public:
	bool			m_bGameRun;
	static bool		m_bFrameRun;
	HCamera*		m_pMainCamera;
	HDebugCamera	m_Camera;
	HShapeLine		m_LineShape;
	HSky			m_SkyBox;
	D3D11_RASTERIZER_DESC rsDesc;

public:
	virtual bool	PreInit();
	virtual bool	Init();
	virtual bool	PostInit();

	virtual bool	PreFrame() {
		return true;
	};
	virtual bool	CameraFrame();
	virtual bool	Frame() {
		return true;
	};
	virtual bool	PostFrame();
	virtual bool	Render() {
		return true;
	};
	virtual bool	PreRender();
	virtual bool	PostRender();

	virtual bool	Release() {
		return true;
	};
	virtual HRESULT DeleteDXResource();
	virtual HRESULT CreateDXResource(UINT w, UINT h);
	bool InitTool(HWND hwnd, HINSTANCE hInstance);
	bool ReleaseTool();
	bool ToolRun();

private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();

public:
	HCore();
	virtual ~HCore();
};


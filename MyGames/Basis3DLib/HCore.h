#pragma once
#include "HDevice.h"
#include "HTimer.h"
#include "HInput.h"
#include "HWrite.h"

class HCore : public HDevice
{
public:
	bool		m_bGameRun;
public:
	virtual bool	PreInit();
	virtual bool	Init();
	virtual bool	PostInit();

	virtual bool	PreFrame() {
		return true;
	};
	virtual bool	Frame() {
		return true;
	};
	virtual bool	PostFrame() {
		return true;
	};
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
private:
	bool	GameInit();
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease();
	bool	GameRun();
public:
	bool	Run();
};


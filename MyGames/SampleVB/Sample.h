#pragma once
#include "HCore.h"
#include "HVertex.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

struct P_VERTEX
{
	HVertex3 pos;
	HVertex3 n;

	P_VERTEX() {};
	P_VERTEX(HVertex3 pos, HVertex3 n = { 0,0,0 })
	{
		this->pos = pos;
		this->n = n;
	}
};


class Sample : public HCore
{

public:
	std::vector<P_VERTEX>  m_VertexList;
	std::vector<DWORD>     m_IndexList;

public:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;

public:
	bool Init() override;
	bool Render() override;
	bool Release() override;

public:
	void CompilerCheck(ID3DBlob* ErrorMsg);

};

HGAME_RUN;
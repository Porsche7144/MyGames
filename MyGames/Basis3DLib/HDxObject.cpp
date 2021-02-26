#include "HDxObject.h"
#include "HTextureManager.h"

namespace HBASIS_CORE_LIB
{

	void ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext)
	{
		if (pd3dDeviceContext == NULL) return;

		ID3D11ShaderResourceView* pSRVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11RenderTargetView* pRTVs[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11DepthStencilView* pDSV = NULL;
		ID3D11Buffer* pBuffers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		ID3D11SamplerState* pSamplers[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		UINT StrideOffset[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		// Shaders
		pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->HSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->DSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->GSSetShader(NULL, NULL, 0);
		pd3dDeviceContext->PSSetShader(NULL, NULL, 0);

		// IA clear
		pd3dDeviceContext->IASetVertexBuffers(0, 16, pBuffers, StrideOffset, StrideOffset);
		pd3dDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		pd3dDeviceContext->IASetInputLayout(NULL);

		// Constant buffers
		pd3dDeviceContext->VSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->HSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->DSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->GSSetConstantBuffers(0, 14, pBuffers);
		pd3dDeviceContext->PSSetConstantBuffers(0, 14, pBuffers);

		// Resources
		pd3dDeviceContext->VSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->HSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->DSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->GSSetShaderResources(0, 16, pSRVs);
		pd3dDeviceContext->PSSetShaderResources(0, 16, pSRVs);

		// Samplers
		pd3dDeviceContext->VSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->HSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->DSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->GSSetSamplers(0, 16, pSamplers);
		pd3dDeviceContext->PSSetSamplers(0, 16, pSamplers);

		// Render targets
		pd3dDeviceContext->OMSetRenderTargets(8, pRTVs, pDSV);

		// States
		FLOAT blendFactor[4] = { 0,0,0,0 };
		pd3dDeviceContext->OMSetBlendState(NULL, blendFactor, 0xFFFFFFFF);
		pd3dDeviceContext->OMSetDepthStencilState(NULL, 0);
		pd3dDeviceContext->RSSetState(NULL);
	}

	HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude,
		LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
		/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult)
	{
		HRESULT hr = S_OK;
		hr = D3DCompileFromFile(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags1, Flags2, ppShader, ppErrorMsgs);
		return hr;
	}

	HRESULT CompileShaderFromFile(const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;
		hr = D3DCompileFromFile(szFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	ID3D11VertexShader*   LoadVertexShaderFile(ID3D11Device*  pd3dDevice,
		const void* pShaderFileData,
		ID3DBlob** ppBlobOut,
		const char *pFuntionName,
		bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11VertexShader* pVertexShader;
		ID3DBlob* pBlob = NULL;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "VS", "vs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "vs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			pBlob = *ppBlobOut;
			if (pBlob == nullptr) return nullptr;
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}

		if (FAILED(hr = pd3dDevice->CreateVertexShader(lpData, dwSize, NULL, &pVertexShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;
		return pVertexShader;
	};

	ID3D11PixelShader*   LoadPixelShaderFile(ID3D11Device*  pd3dDevice,
		const void* pShaderFileData,
		const char *pFuntionName,
		bool bBinary, ID3DBlob** pRetBlob)
	{
		HRESULT hr = S_OK;
		ID3D11PixelShader* pPixelShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;
		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "PS", "ps_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "ps_5_0", &pBlob)))
				{
					return nullptr;
				}
			}

			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreatePixelShader(lpData, dwSize, NULL, &pPixelShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (pRetBlob == nullptr)
		{
			pBlob->Release();
		}
		else
		{
			*pRetBlob = pBlob;
		}
		return pPixelShader;
	}

	ID3D11GeometryShader*  LoadGeometryShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData,
		ID3DBlob** ppBlobOut,
		const char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11GeometryShader* pGeometryShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "GS", "gs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "gs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateGeometryShader(lpData, dwSize, NULL, &pGeometryShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		//D3DCreateBlob(dwSize, ppBlobOut);
		return pGeometryShader;
	}

	ID3D11HullShader* LoadHullShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, const char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11HullShader* pHullShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "HS", "hs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "hs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateHullShader(lpData, dwSize, NULL, &pHullShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pHullShader;
	};

	ID3D11DomainShader*  LoadDomainShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, const char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11DomainShader* pDomainShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "DS", "ds_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "ds_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateDomainShader(lpData, dwSize, NULL, &pDomainShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pDomainShader;
	};

	ID3D11ComputeShader*  LoadComputeShaderFile(ID3D11Device*  pd3dDevice, const void* pShaderFileData, ID3DBlob** ppBlobOut, const char *pFuntionName, bool bBinary)
	{
		HRESULT hr = S_OK;
		ID3D11ComputeShader* pComputeShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		DWORD dwSize = 0;
		LPCVOID lpData = NULL;

		if (bBinary == false)
		{
			if (pFuntionName == 0)
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, "CS", "cs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			else
			{
				if (FAILED(hr = CompileShaderFromFile((TCHAR*)pShaderFileData, pFuntionName, "cs_5_0", &pBlob)))
				{
					return nullptr;
				}
			}
			//pBlob = *ppBlobOut;
			dwSize = (DWORD)pBlob->GetBufferSize();
			lpData = pBlob->GetBufferPointer();
		}
		else
		{
			dwSize = sizeof(pShaderFileData);
			lpData = pShaderFileData;
		}
		if (FAILED(hr = pd3dDevice->CreateComputeShader(lpData, dwSize, NULL, &pComputeShader)))
		{
			pBlob->Release();
			return nullptr;
		}
		if (ppBlobOut == nullptr)
			pBlob->Release();
		else
			*ppBlobOut = pBlob;

		return pComputeShader;
	};

	ID3D11InputLayout* CreateInputlayout(ID3D11Device*  pd3dDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements)
	{
		HRESULT hr = S_OK;
		ID3D11InputLayout* pInutLayout = nullptr;
		if (FAILED(hr = pd3dDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInutLayout)))
		{
			H(hr);
			return nullptr;
		}
		return pInutLayout;
	}

	ID3D11Buffer* CreateVertexBuffer(
		ID3D11Device*  pd3dDevice,
		void *vertices,
		UINT iNumVertex,
		UINT iVertexSize,
		bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iVertexSize * iNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		if (vertices != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateIndexBuffer(ID3D11Device*  pd3dDevice, void *indices, UINT iNumIndex,
		UINT iSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumIndex;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		InitData.pSysMem = indices;
		if (indices != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateConstantBuffer(ID3D11Device*  pd3dDevice, void *data, UINT iNumIndex,
		UINT iSize, bool bDynamic)
	{
		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		bd.ByteWidth = iSize * iNumIndex;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		InitData.pSysMem = data;
		if (data != NULL)
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		else
		{
			if (FAILED(hr = pd3dDevice->CreateBuffer(&bd, NULL, &pBuffer)))
			{
				H(hr);
				return nullptr;
			}
		}
		return pBuffer;
	}

	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath)
	{
		HRESULT hr = S_OK;
		if (strFilePath == NULL) return nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		ComPtr<ID3D11Resource> res;
		DDS_ALPHA_MODE alphaMode;
		/*hr = DirectX::CreateWICTextureFromFileEx(pDevice,
			strFilePath,
			0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS,
			WIC_LOADER_DEFAULT,
			res.GetAddressOf(), &pSRV);*/

		hr = DirectX::CreateWICTextureFromFile(pDevice, strFilePath, res.GetAddressOf(), &pSRV);
		if (FAILED(hr))
		{
			hr = DirectX::CreateDDSTextureFromFile(pDevice, strFilePath, res.GetAddressOf(), &pSRV, 0, &alphaMode);
		}
		if (FAILED(hr))
		{
			H(hr);
			return nullptr;
		}
		return pSRV;
	}

	ID3D11ShaderResourceView*	CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext*    pContext, const TCHAR* strFilePath)
	{
		HRESULT hr = S_OK;
		if (strFilePath == NULL) return nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		ComPtr<ID3D11Resource> res;
		DDS_ALPHA_MODE alphaMode;
		/*hr = DirectX::CreateWICTextureFromFileEx(pDevice,
			strFilePath,
			0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_GENERATE_MIPS,
			WIC_LOADER_DEFAULT,
			res.GetAddressOf(), &pSRV);*/

		hr = DirectX::CreateWICTextureFromFile(pDevice, pContext, strFilePath, res.GetAddressOf(), &pSRV);
		if (FAILED(hr))
		{
			hr = DirectX::CreateDDSTextureFromFile(pDevice, pContext, strFilePath, res.GetAddressOf(), &pSRV, 0, &alphaMode);
		}
		if (FAILED(hr))
		{
			H(hr);
			return nullptr;
		}
		return pSRV;
	}

	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight)
	{
		HRESULT hr;
		ID3D11DepthStencilView* pDSV = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
		D3D11_TEXTURE2D_DESC DescDepth;
		DescDepth.Width = dwWidth;
		DescDepth.Height = dwHeight;
		DescDepth.MipLevels = 1;
		DescDepth.ArraySize = 1;
		DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DescDepth.SampleDesc.Count = 1;
		DescDepth.SampleDesc.Quality = 0;
		DescDepth.Usage = D3D11_USAGE_DEFAULT;
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DescDepth.CPUAccessFlags = 0;
		DescDepth.MiscFlags = 0;
		if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
		{
			return nullptr;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DescDepth.Format;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice = 0;

		if (FAILED(hr = pDevice->CreateDepthStencilView(
			pDSTexture.Get(), &dsvd, &pDSV)))
		{
			return nullptr;
		}
		return pDSV;
	}


	void HDxObject::CompilerCheck(ID3DBlob* ErrorMsg)
	{
		C_STR szMsg = (char*)ErrorMsg->GetBufferPointer();
		T_STR szError = to_mw(szMsg);
		MessageBox(NULL, szError.c_str(), L"ERROR", MB_OK);
	}

	bool HDxObject::Create(ID3D11Device * pDevice, T_STR vs, T_STR ps, T_STR texture)
	{
		g_pd3dDevice = pDevice;

		CreateVertexData();
		CreateVertexBuffer();
		CreateConstantBuffer();
		CreateIndexData();
		CreateIndexBuffer();
		LoadShader(vs, ps);
		CreateInputLayOut();
		LoadTexture(texture);

		return true;
	}

	bool HDxObject::CreateVertexBuffer()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		/*UINT ByteWidth;
		D3D11_USAGE Usage;
		UINT BindFlags;
		UINT CPUAccessFlags;
		UINT MiscFlags;
		UINT StructureByteStride;*/
		bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		// Vertex 버퍼로 바인딩
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_VertexList.at(0);
		HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	bool HDxObject::CreateIndexBuffer()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		// 인덱스 버퍼로 바인딩
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_IndexList.at(0);
		HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	bool HDxObject::CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(HDataCB);
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		// Constant 버퍼로 바인딩
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_cbData;
		HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_ConstantBuffer);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	bool HDxObject::CreateVertexData()
	{
		return true;
	}

	bool HDxObject::CreateIndexData()
	{
		return true;
	}

	bool HDxObject::CreateInputLayOut()
	{
		// LayOut
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT, 0, 40,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT iNumElement = sizeof(layout) / sizeof(layout[0]);
		HRESULT hr = g_pd3dDevice->CreateInputLayout(layout, iNumElement, pVSObj->GetBufferPointer(),
			pVSObj->GetBufferSize(), &m_pInputLayout);
		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	bool HDxObject::Update(ID3D11DeviceContext* pd3dContext)
	{

		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = pd3dContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		if (SUCCEEDED(hr))
		{
			HDataCB* pData = (HDataCB*)ms.pData;
			pData->matWorld = m_matWorld.Transpose();
			pData->matView = m_matView.Transpose();
			pData->matProject = m_matProject.Transpose();

			pData->vColor[0] = cosf(g_fGameTimer);
			pData->vColor[1] = sinf(g_fGameTimer);
			pData->vColor[2] = 1.0f - cosf(g_fGameTimer);
			pData->vColor[3] = 1;
			pData->vTime[0] = cosf(g_fGameTimer) * 0.5f + 0.5f;
			pData->vTime[1] = g_fGameTimer;
			// map 사용 후 unmap 쓰레드. mutex와 같음
			pd3dContext->Unmap(m_ConstantBuffer, 0);
		}

		return true;
	}

	bool HDxObject::LoadShader(T_STR vs, T_STR ps)
	{
		// 쉐이더
		ID3DBlob* pPSObj;
		ID3DBlob* pErrorMsg;

		// Vertex 쉐이더
		HRESULT hr = D3DCompileFromFile(vs.c_str(), NULL, NULL, m_szVertexShader.c_str(), "vs_5_0", 0, 0, &pVSObj, &pErrorMsg);
		if (FAILED(hr))
		{
			CompilerCheck(pErrorMsg);
			return false;
		}
		hr = g_pd3dDevice->CreateVertexShader(pVSObj->GetBufferPointer(), pVSObj->GetBufferSize(), NULL,
			&m_pVertexShader);
		if (FAILED(hr))
		{
			return false;
		}
		// Pixel 쉐이더
		hr = D3DCompileFromFile(ps.c_str(), NULL, NULL, m_szPixelShader.c_str(), "ps_5_0", 0, 0, &pPSObj, &pErrorMsg);
		if (FAILED(hr))
		{
			CompilerCheck(pErrorMsg);
			return false;
		}
		hr = g_pd3dDevice->CreatePixelShader(pPSObj->GetBufferPointer(), pPSObj->GetBufferSize(), NULL,
			&m_pPixelShader);
		if (FAILED(hr))
		{
			return false;
		}

		if (pPSObj)
		{
			pPSObj->Release();
		}

		return true;
	}

	bool HDxObject::LoadTexture(T_STR texture)
	{
		// 텍스쳐 로드
		m_pTexture = g_TextureMgr.Load(g_pd3dDevice, texture.c_str());
		if (m_pTexture == nullptr) return false;

		return true;
	}

	bool HDxObject::SetMatrix(Matrix * pWorld, Matrix * pView, Matrix * pProj)
	{
		if (pWorld != nullptr)
		{
			m_matWorld = *pWorld;
		}
		else
		{
			m_matWorld = m_matScale * m_matRotation;
			m_matWorld._41 = m_vPos.x;
			m_matWorld._42 = m_vPos.y;
			m_matWorld._43 = m_vPos.z;
		}
		if (pView != nullptr)
		{
			m_matView = *pView;
		}
		if (pProj != nullptr)
		{
			m_matProject = *pProj;
		}

		m_vLook.x = m_matWorld._31;
		m_vLook.y = m_matWorld._32;
		m_vLook.z = m_matWorld._33;
		m_vUp.x = m_matWorld._21;
		m_vUp.y = m_matWorld._22;
		m_vUp.z = m_matWorld._23;
		m_vRight.x = m_matWorld._11;
		m_vRight.y = m_matWorld._12;
		m_vRight.z = m_matWorld._13;

		m_vLook.Normalize();
		m_vUp.Normalize();
		m_vRight.Normalize();

		return true;
	}

	bool HDxObject::Init()
	{
		m_matWorld = Matrix::Identity;
		m_matView = Matrix::Identity;
		m_matProject = Matrix::Identity;

		return true;
	}

	bool HDxObject::Frame()
	{
		return true;
	}

	bool HDxObject::PreRender(ID3D11DeviceContext * pContext)
	{
		return true;
	}

	bool HDxObject::Render(ID3D11DeviceContext* pContext)
	{
		Update(pContext);
		PreRender(pContext);
		UINT iStride = sizeof(PNCT_VERTEX);
		UINT iOffset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &iStride, &iOffset);
		pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pContext->IASetInputLayout(m_pInputLayout);

		pContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
		pContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);

		pContext->VSSetShader(m_pVertexShader, NULL, 0);
		pContext->PSSetShader(m_pPixelShader, NULL, 0);
		pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iTopology);
		if (m_pTexture != nullptr)
		{
			pContext->PSSetShaderResources(0, 1, &m_pTexture->m_pTextureSRV);
		}
		// Vertex Draw
		// g_pImmediateContext->Draw(m_VertexList.size(), 0);

		PostRender(pContext);

		return true;
	}

	bool HDxObject::PostRender(ID3D11DeviceContext * pContext)
	{
		pContext->DrawIndexed(m_IndexList.size(), 0, 0);
		return true;
	}

	bool HDxObject::Release()
	{
		m_ConstantBuffer->Release();
		m_pVertexBuffer->Release();
		m_pIndexBuffer->Release();
		m_pInputLayout->Release();
		m_pVertexShader->Release();
		m_pPixelShader->Release();

		return true;
	}

	HDxObject::HDxObject()
	{
		m_szVertexShader = "VS";
		m_szPixelShader = "PS";
		m_iTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	HDxObject::~HDxObject()
	{
	}

}
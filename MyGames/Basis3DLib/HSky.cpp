#include "HSky.h"

bool HSky::Create(ID3D11Device * pDevice, T_STR vs, T_STR ps, T_STR texture)
{
	if (HObject::Create(pDevice, vs, ps, texture) == false)
	{
		MessageBox(0, _T("SKY ���� ����"), _T("Fatal Error"), MB_OK);
		return false;
	}

	m_pSkyPS.Attach(HBASIS_CORE_LIB::LoadPixelShaderFile(pDevice, ps.c_str(), "PS_TextureIndex"));

	return true;
}

bool HSky::LoadTexture(T_STR texture)
{
	HRESULT hr = S_OK;

	const TCHAR* skyTexture[] =
	{
		L"../../Image/sky/Space/corona_rt.png",
		L"../../Image/sky/Space/corona_lf.png",
		L"../../Image/sky/Space/corona_ft.png",
		L"../../Image/sky/Space/corona_bk.png",
		L"../../Image/sky/Space/corona_up.png",
		L"../../Image/sky/Space/corona_dn.png",
	};
	/*	L"../../Image/sky/Space/corona_rt.png",
		L"../../Image/sky/Space/corona_lf.png",
		L"../../Image/sky/Space/corona_ft.png",
		L"../../Image/sky/Space/corona_bk.png",
		L"../../Image/sky/Space/corona_up.png",
		L"../../Image/sky/Space/corona_dn.png",*/

	int iNumTexture = sizeof(skyTexture) / sizeof(skyTexture[0]);

	for (int iTexture = 0; iTexture < iNumTexture; iTexture++)
	{
		m_pTexSRV[iTexture].Attach(HBASIS_CORE_LIB::
				CreateShaderResourceView(g_pd3dDevice, g_pImmediateContext, skyTexture[iTexture]));
	}

	return true;
}

bool HSky::Render(ID3D11DeviceContext* pContext)
{
	m_matWorld = Matrix::CreateScale(100.0f, 100.0f, 100.0f);

	// ����� �׻� ���� 0, 0, 0(����)�� ��ġ�ؾ� ��.
	Matrix matSkyView = m_matView;
	matSkyView._41 = 0;
	matSkyView._42 = 0;
	matSkyView._43 = 0;

	g_pImmediateContext->RSSetState(HDxState::g_pRSNoneCullSolid);
	g_pImmediateContext->PSSetSamplers(0, 1, &HDxState::g_pSSClampPoint);
	g_pImmediateContext->OMSetDepthStencilState(HDxState::g_pDSSDepthDisable, 0);

	SetMatrix(&m_matWorld, &matSkyView, &m_matProject);
	HDxObject::Render(pContext);

	return true;
}

bool HSky::PostRender(ID3D11DeviceContext* pContext)
{
	if (m_bRenderType)
	{
		// ������ VB, IB�� ����Ͽ� ���徿 ������ �ϴ� ���
		for (int i = 0; i < MAX_SKY_TEXTURE; i++)
		{
			if (m_pTexSRV[i] == nullptr) break;

			pContext->PSSetShaderResources(0, 1, m_pTexSRV[i].GetAddressOf());
			pContext->PSSetShader(m_pSkyPS.Get(), NULL, 0);

			// ������ �� �ε��� ���� ��, �ε��� ���� ����, ���ؽ� ���� ����
			pContext->DrawIndexed(6, 6 * i, 0);
		}
	}
	else
	{
		// �ϳ��� VB, IB�� ������ �ѹ��� ������ �ϴ� ���
		pContext->PSSetShaderResources(1, 6, m_pTexSRV[0].GetAddressOf());
		pContext->PSSetShader(m_pSkyPS.Get(), NULL, 0);

		// ������ �� �ε��� ���� ��, �ε��� ���� ����, ���ؽ� ���� ����
		pContext->DrawIndexed(36, 0, 0);
	}

	return true;
}
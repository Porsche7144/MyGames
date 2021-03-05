#include "Sample.h"

bool Sample::Init()
{

	HMapDesc MapDesc;
	MapDesc.iNumCols = 64;
	MapDesc.iNumRows = 64;
	MapDesc.fCellDistance = 1.0f;
	MapDesc.fScaleHeight = 0.1f;
	MapDesc.szTextFile = L"../../Image/data/map/castle.jpg";
	MapDesc.szVS = L"VS.txt";
	MapDesc.szPS = L"PS.txt";
	if (!m_CustomMap.CreateMap(g_pd3dDevice, MapDesc))
	{
		return false;
	}

	
	m_pMainCamera->CreateViewMatrix(Vector3(0.0f, 100.0f, 50.0f), Vector3(0.0f, 0.0f, 1.0f));
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_pMainCamera->CreateProjectionMatrix(1.0f, 1000.0f, HBASIS_PI * 0.25f, fAspect);

	return true;
}

bool Sample::Frame()
{

	float t = cosf(g_fGameTimer * 0.5f);
	for (int z = 0; z < m_CustomMap.m_iNumRows; z++)
	{
		for (int x = 0; x < m_CustomMap.m_iNumCols; x++)
		{
			int iIndex = z * m_CustomMap.m_iNumCols + x;
			float fCos = cosf(t * x);
			float fSin = sinf(t * z);
			m_CustomMap.m_VertexList[iIndex].p.y = fCos + fSin;
		}
	}

	// UpdateSubresource�� ���� ������Ʈ
	g_pImmediateContext->UpdateSubresource(m_CustomMap.m_pVertexBuffer, 0, NULL, &m_CustomMap.m_VertexList.at(0), 0, 0);
	m_pMainCamera->Frame();
	m_CustomMap.Frame();

	return true;
}

bool Sample::Render()
{

	m_CustomMap.SetMatrix(&m_pMainCamera->m_matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProject);
	m_CustomMap.Render(g_pImmediateContext);

	return true;
}


bool Sample::Release()
{
	m_CustomMap.Release();
	return true;
}

Sample::Sample()
{
	m_pMainCamera = nullptr;
}
Sample::~Sample()
{

}
#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	// 셰이더 초기화
	GET_SINGLE(ShaderManager)->AddShader(L"Terrain", L"./Resource/Shader/Terrain.hpp");

	information.min = *(GET_SINGLE(BufferManager)->GetMin(L"Terrain"));
	information.max = *(GET_SINGLE(BufferManager)->GetMax(L"Terrain"));

	// 버텍스 정보 초기화
	const int& vertexCnt = GET_SINGLE(BufferManager)->GetVertexCount(L"Terrain");
	if (vertexCnt == -1) {
		MSGBOX(L"Terrain 버텍스 개수 초기화 실패");
		return E_FAIL;
	}

	vtxTex = new VTXTEX[vertexCnt];
	GET_SINGLE(BufferManager)->CopyVertexTextureInfo(L"Terrain", vtxTex);

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);

	return S_OK;
}

void Terrain::Progress()
{
}

void Terrain::Oncolide()
{

}
void Terrain::Render()
{
	GameObject::Render();

	// 셰이더 변수 설정
	LPD3DXEFFECT effect = GET_SINGLE(ShaderManager)->GetShader(L"Terrain");
	effect->SetMatrix("gWorldMatrix", &world);

	D3DXMATRIX matView, matProj;
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("gWorldMatrix", &world);
	effect->SetMatrix("gViewMatrix", &matView);
	effect->SetMatrix("gProjectionMatrix", &matProj);

	// 메쉬 렌더링
	GET_SINGLE(ShaderManager)->Shader_Begin(L"Terrain");

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GET_SINGLE(BufferManager)->Render(L"Terrain");
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

	GET_SINGLE(ShaderManager)->Shader_End(L"Terrain");
}

void Terrain::Release()
{
	SAFE_DELETE_ARRAY(vtxTex);
	SAFE_DELETE_ARRAY(vtxCol);
}

Terrain::Terrain()
{
}

Terrain::Terrain(const TCHAR* _key, INFO _info)
	:GameObject(_key, _info)
{
}

Terrain::~Terrain()
{
	Release();
}

#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	// 생성된 터레인 메쉬 복제
	GET_SINGLE(MeshManager)->CloneMesh(L"Terrain",&information.mesh);

	// 터레인 셰이더 추가
	//GET_SINGLE(ShaderManager)->AddShader(L"Terrain", L"./Resource/Shader/Terrain.hpp");

	// 충돌 정점 설정
	information.min = *GET_SINGLE(MeshManager)->GetMin(L"Terrain");
	information.max = *GET_SINGLE(MeshManager)->GetMax(L"Terrain");

	vtxTex = new VTXTEX[information.mesh->GetNumVertices()];
	GET_SINGLE(MeshManager)->CopyVertexInfo_VTXTEX(L"Terrain", vtxTex);

	D3DXMatrixIdentity(&information.world);
	return S_OK;
}

void Terrain::Progress()
{
}

void Terrain::Render()
{
	GameObject::Render();

	GET_SINGLE(ShaderManager)->Shader_Begin(L"Terrain");

	// 메쉬 렌더링
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	information.mesh->DrawSubset(0);
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

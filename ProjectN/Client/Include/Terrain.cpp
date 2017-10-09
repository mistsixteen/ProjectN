#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	// 생성된 터레인 메쉬 복제
	GET_SINGLE(MeshManager)->CloneMesh(L"Terrain",&information.mesh);

	// 터레인 셰이더 추가
	GET_SINGLE(ShaderManager)->AddShader(L"Terrain", L"./Resource/Shader/Terrain.hpp");

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

	// 셰이더 변수 설정
	LPD3DXEFFECT effect = GET_SINGLE(ShaderManager)->GetShader(L"Terrain");
	effect->SetMatrix("gWorldMatrix", &information.world);

	D3DXMATRIX matView, matProj;
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("gWorldMatrix", &information.world);
	effect->SetMatrix("gViewMatrix", &matView);
	effect->SetMatrix("gProjectionMatrix", &matProj);

	// 메쉬 렌더링
	GET_SINGLE(ShaderManager)->Shader_Begin(L"Terrain");
	GET_SINGLE(MeshManager)->Mesh_Render(L"Terrain");
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

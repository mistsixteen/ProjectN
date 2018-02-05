#include "stdafx.h"
#include "Background.h"


HRESULT Background::Initialize(void)
{
	// 큐브 텍스쳐 초기화
	if (FAILED(GET_SINGLE(TextureManager)->AddTexture(L"./Resource/Texture/Environment/Background/Space.dds"
														, TEXTYPE_CUBE, L"Background", L"Space"))) {
		return E_FAIL;
	}

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 1024.f, 1024.f, 1024.f);

	return S_OK;
}

void Background::Progress(void)
{
	INFO playerInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	information.position = playerInfo.position;
}

void Background::Render(void)
{
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	GameObject::Render();
	GET_SINGLE(DXFramework)->GetDevice()->SetTexture(0,
	GET_SINGLE(TextureManager)->GetTexture(L"Background", L"Space", 0)->cubeTexture);
	GET_SINGLE(BufferManager)->Render(L"Background");

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void Background::Release(void)
{
}

void Background::Oncolide(void)
{

}

Background::Background()
{
}

Background::Background(const TCHAR * _key, INFO _info)
	:GameObject(_key, _info)
{
}


Background::~Background()
{
	Release();
}

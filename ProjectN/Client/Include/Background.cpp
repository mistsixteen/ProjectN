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
	D3DXMatrixScaling(&scale, 512.f, 512.f, 512.f);

	return S_OK;
}

void Background::Progress(void)
{
}

void Background::Render(void)
{
	GameObject::Render();
	GET_SINGLE(DXFramework)->GetDevice()->SetTexture(0, 
	GET_SINGLE(TextureManager)->GetTexture(L"Background", L"Space", 0)->cubeTexture);

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	GET_SINGLE(BufferManager)->Render(L"Background");
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
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

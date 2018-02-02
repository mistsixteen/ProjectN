#include "stdafx.h"
#include "Aim.h"

HRESULT Aim::Initialize(void)
{
	// 에임 텍스쳐 초기화(임시 : 몬스터)
	if (FAILED(GET_SINGLE(TextureManager)->AddTexture(L"./Resource/Texture/UI/stand.png",
														TEXTYPE_GENERAL, L"Aim", L"1"))) {
		return E_FAIL;
	}


	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	information.position = { WINSIZEX / 2.f, WINSIZEY / 2.f, 0.f};

	return S_OK;
}

void Aim::Progress(void)
{
}

void Aim::Render(void)
{
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	// D3DRS_ALPHATESTENABLE
	// 기본이 FALSE 
	// TRUE는 알파 테스팅을 사용 하겠다
	// Z버퍼에 기입을 할건지 말건지 선택
	// D3DRS_ALPHAREF 알파테스팅중 사용할 알파 수치
	// D3DRS_ALPHAFUNC 알파값 크기 비교해서 높은놈만 출력하게
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	const TEXINFO* texInfo = GET_SINGLE(TextureManager)->GetTexture(L"Aim", L"1");
	if (texInfo == NULL)
		return;

	GET_SINGLE(DXFramework)->GetDevice()->SetTexture(0, texInfo->texture);
	UI::Render();

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


}

void Aim::Release(void)
{
}

Aim::Aim()
{
}

Aim::Aim(const TCHAR * _key)
	:UI(_key)
{
}


Aim::~Aim()
{
	Release();
}
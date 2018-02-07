#include "stdafx.h"
#include "Aim.h"

HRESULT Aim::Initialize(void)
{
	// ���� �ؽ��� �ʱ�ȭ(�ӽ� : ����)
	if (FAILED(GET_SINGLE(TextureManager)->AddTexture(L"./Resource/Texture/UI/Aim.png",
														TEXTYPE_GENERAL, L"UI", L"Aim"))) {
		return E_FAIL;
	}

	// ��Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&this->world);
	D3DXMatrixIdentity(&this->view);

	information.position = {WINSIZEX / 2.f, WINSIZEY / 2.f, 0.f};
	converPos = information.position;
	converPos.y *= -1.f;
	converPos.y += WINSIZEY / 2.f;
	converPos.x -= WINSIZEX / 2.f;
	sizeX = 80.f;
	sizeY = 80.f;

	/*SetRect(&rc,
		int(information.position.x - sizeX / 2.f),
		int(information.position.y - sizeY / 2.f),
		int(information.position.x + sizeX / 2.f),
		int(information.position.y + sizeY / 2.f));*/

	return S_OK;
}

void Aim::Progress(void)
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, information.position.x
		, information.position.y, information.position.z);

	//���� ������� ����
	D3DXMatrixOrthoLH(&this->projection, float(WINSIZEX), float(WINSIZEY), 0.f, 1.f);
	//D3DXVECTOR3 vTest(-WINSIZEX / 2.f, -WINSIZEY / 2.f, 0);

	//������ 0,0 ������ǥ 0,0
	//������ 400,300 ������ǥ 1,1
	//������ -400,-300 ������ǥ -1,-1
	//D3DXVec3TransformCoord(&vTest, &vTest, &this->projection);

}

void Aim::Render(void)
{
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	this->view._11 = sizeX / 2.f;
	this->view._22 = sizeY / 2.f;
	this->view._33 = 1.f;

	this->view._41 = converPos.x;
	this->view._42 = converPos.y;

	D3DXMATRIX	view, projection;
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &view);
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &projection);

	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &this->view);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_PROJECTION, &this->projection);

	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_WORLD, &this->world);
	GET_SINGLE(DXFramework)->GetDevice()->SetTexture(0, 
	GET_SINGLE(TextureManager)->GetTexture(L"UI", L"Aim")->texture);
	GET_SINGLE(BufferManager)->Render(L"UI");

	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &view);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_PROJECTION, &projection);

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
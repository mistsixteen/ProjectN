#include "stdafx.h"
#include "Bullet.h"


HRESULT Bullet::Initialize(void)
{
	information.min = *(GET_SINGLE(MeshManager)->GetMin(L"Bullet"));
	information.max = *(GET_SINGLE(MeshManager)->GetMax(L"Bullet"));
	active = false;

	// ��Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 5.f, 5.f, 5.f);

	// �ʱ� �ɼ� �ʱ�ȭ
	information.speed = 10.f;
	information.attack = 1.f;
	bgMin = GET_SINGLE(ObjectManager)->GetGameObject(L"Background")->GetInfo().min;
	bgMax = GET_SINGLE(ObjectManager)->GetGameObject(L"Background")->GetInfo().max;

	// GET_SINGLE(TextureManager)->AddTexture(L"./Resource/Texture/Bullet/beams.png", TEXTYPE_GENERAL, L"Bullet", L"Beam");

	initBullet();

	return S_OK;
}

bool Bullet::isFired() const
{
	return active;
}

void Bullet::initBullet()
{
	INFO playerInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();

	information.position = playerInfo.position;
	information.direction = playerInfo.look;

	active = true;
}


void Bullet::Progress(void)
{
	if (isFired()) {
		information.position += information.direction
								* information.speed * GET_SINGLE(TimeManager)->GetTime();

		if (information.position.x < bgMin.x || bgMax.x < information.position.x
			|| information.position.y < bgMin.y || bgMax.y < information.position.y
			|| information.position.z < bgMin.z || bgMax.z < information.position.z) {
			//initBullet();
		}

		GameObject* dest = GET_SINGLE(ObjectManager)->GetInterectedObject(information);
		if (dest != nullptr) {
			dest->GetInfo().damage = information.attack;
			//initBullet();
		}
	}

}

void Bullet::Render(void)
{
	GameObject::Render();

	// 임의로 타겟 셰이더 빌려옴
	LPD3DXEFFECT effect = GET_SINGLE(ShaderManager)->GetShader(L"Target");

	D3DXMATRIX matView, matProj;
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("gWorldMatrix", &world);
	effect->SetMatrix("gViewMatrix", &matView);
	effect->SetMatrix("gProjectionMatrix", &matProj);
	effect->SetVector("gColor", &D3DXVECTOR4(1.f,1.f,0.f,1.f));

	GET_SINGLE(ShaderManager)->Shader_Begin(L"Target");

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
	GET_SINGLE(MeshManager)->Render(L"Bullet");
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

	GET_SINGLE(ShaderManager)->Shader_End(L"Target");
}

void Bullet::Release(void)
{

}


void Bullet::Oncolide(void)
{

}

Bullet::Bullet()
{

}

Bullet::Bullet(const TCHAR * _key, INFO _info)
	:GameObject(_key, _info)
{

}

Bullet::~Bullet()
{

}

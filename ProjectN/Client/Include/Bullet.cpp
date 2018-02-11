#include "stdafx.h"
#include "Bullet.h"


HRESULT Bullet::Initialize(void)
{
	information.min = *(GET_SINGLE(MeshManager)->GetMin(L"Bullet"));
	information.max = *(GET_SINGLE(MeshManager)->GetMax(L"Bullet"));

	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 5.f, 5.f, 5.f);

	information.speed = 10.f;
	information.attack = 1.f;

	printf("Pos : %.3f, %.3f, %.3f\n", information.position.x, information.position.y, information.position.z);

	isFired = true;

	return S_OK;
}

void Bullet::Progress(void)
{
	if (isFired) {
		information.position += information.look * information.speed * GET_SINGLE(TimeManager)->GetTime();

		GameObject* dest = GET_SINGLE(ObjectManager)->GetInterectedObject(information);
		if (dest != nullptr) {
			dest->GetInfo().damage = information.attack;
			isFired = false;
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
	GET_SINGLE(MeshManager)->Render(L"Bullet");
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

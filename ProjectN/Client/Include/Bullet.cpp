#include "stdafx.h"
#include "Bullet.h"


HRESULT Bullet::Initialize(void)
{
	information.min = *(GET_SINGLE(MeshManager)->GetMin(L"Bullet"));
	information.max = *(GET_SINGLE(MeshManager)->GetMax(L"Bullet"));

	// ��Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);

	// �ʱ� �ɼ� �ʱ�ȭ
	information.speed = 30.f;
	information.attack = 1.f;
	bgMin = GET_SINGLE(ObjectManager)->GetGameObject(L"Background")->GetInfo().min;
	bgMax = GET_SINGLE(ObjectManager)->GetGameObject(L"Background")->GetInfo().max;

	return S_OK;
}

bool Bullet::isFired() const
{
	return information.direction.x && information.direction.y && information.direction.z;
}

void Bullet::initBullet()
{
	INFO playerInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	information.position = playerInfo.position;
	information.direction = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void Bullet::Progress(void)
{
	// �߻� �� �̵�
	if (isFired()) {
		information.position += GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo().direction
								* information.speed * GET_SINGLE(TimeManager)->GetTime();

		// ���� ��Ż
		if (information.position.x < bgMin.x || bgMax.x < information.position.x
			|| information.position.y < bgMin.y || bgMax.y < information.position.y
			|| information.position.z < bgMin.z || bgMax.z < information.position.z) {
			initBullet();
		}
		// ������Ʈ �ǰ�
		GameObject* dest = GET_SINGLE(ObjectManager)->GetInterectedObject(information);
		if (dest != nullptr) {
			dest->GetInfo().damage = information.attack;
			initBullet();
		}
	}
	else {
		initBullet();
	}
}

void Bullet::Render(void)
{
	GameObject::Render();

	// �߻� �ÿ��� ���
	if (isFired()) {
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		GET_SINGLE(MeshManager)->Render(L"Bullet");
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

void Bullet::Release(void)
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

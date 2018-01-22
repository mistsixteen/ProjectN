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
	// �߻� �� �̵�
	if (isFired()) {
		information.position += information.direction
								* information.speed * GET_SINGLE(TimeManager)->GetTime();

		// ���� ��Ż
		if (information.position.x < bgMin.x || bgMax.x < information.position.x
			|| information.position.y < bgMin.y || bgMax.y < information.position.y
			|| information.position.z < bgMin.z || bgMax.z < information.position.z) {
			//initBullet();
		}
		// ������Ʈ �ǰ�
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

	// �߻� �ÿ��� ���
	const TEXINFO* texInfo = GET_SINGLE(TextureManager)->GetTexture(L"Bullet", L"Beam");
	if (texInfo == NULL) {
		return;
	}

	RECT frame = {210, 260, 310, 400};
	D3DXVECTOR3 center = { (frame.right - frame.left) * 0.5f, (frame.bottom - frame.top) * 0.5f, 0.f};
	GET_SINGLE(DXFramework)->GetSprite()->SetTransform(&world);
	if (FAILED(GET_SINGLE(DXFramework)->GetSprite()->Draw(texInfo->texture, &frame, &center, NULL, D3DCOLOR_ARGB(255, 255, 255, 255)))) {
		MSGBOX(L"2");
	}
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

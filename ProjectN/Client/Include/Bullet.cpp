#include "stdafx.h"
#include "Bullet.h"


HRESULT Bullet::Initialize(void)
{
	if (FAILED(GET_SINGLE(TextureManager)->AddTexture(L"./Resource/Texture/Bullet/beams.png", TEXTYPE_GENERAL, L"Bullet", L"Beam"))) {
		return E_FAIL;
	}

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 5.f, 5.f, 5.f);

	// 초기 옵션 초기화
	information.speed = 30.f;
	information.attack = 1;
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
	//// 발사 시 이동
	//if (isFired()) {
	//	information.position += GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo().direction
	//							* information.speed * GET_SINGLE(TimeManager)->GetTime();

	//	// 범위 이탈
	//	if (information.position.x < bgMin.x || bgMax.x < information.position.x
	//		|| information.position.y < bgMin.y || bgMax.y < information.position.y
	//		|| information.position.z < bgMin.z || bgMax.z < information.position.z) {
	//		initBullet();
	//	}

	//	// 오브젝트 피격
	//	map<const TCHAR*, list<GameObject*>> objects = GET_SINGLE(ObjectManager)->GetObjectMap();
	//	for (auto iterMap = objects.begin(); iterMap != objects.end(); ++iterMap)
	//	{
	//		if ((*iterMap).first != L"Terrain" && (*iterMap).first != L"Background")
	//		{
	//			for (auto iterList = (*iterMap).second.begin();
	//				iterList != (*iterMap).second.end(); ++iterList)
	//			{
	//				D3DXVECTOR3 distance = (*iterList)->GetInfo().position - information.position;
	//				if (D3DXVec3Length(&distance) < 1.5f) {

	//				}
	//			}
	//		}
	//	}
	//}
	//else {
	//	initBullet();
	//}
}

void Bullet::Render(void)
{
	GameObject::Render();

	// 발사 시에만 출력
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

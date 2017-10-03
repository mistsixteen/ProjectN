#include "stdafx.h"
#include "Player.h"

#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	// 생선된 캐릭터 메쉬 복제
	GET_SINGLE(MeshManager)->CloneMesh(L"Player", &information.mesh);
	
	// 충돌 정점 설정
	information.min = *GET_SINGLE(MeshManager)->GetMin(L"Player");
	information.max = *GET_SINGLE(MeshManager)->GetMax(L"Player");

	// 매트릭스 초기화
	D3DXMatrixIdentity(&information.world);

	// 플레이어 속도 초기화
	information.speed = 10.f;

	// 비행 상태 초기화
	flying = true;
	
	// 낙하 속도 초기화
	fallingSpeed = 0.f;

	return S_OK;
}

void Player::Progress(void)
{
	prevPos = information.position;
	if (GET_SINGLE(DXInput)->PushRight())
		KeyCheck();
	SetPosition();
}

void Player::SetPosition()
{
	float height = GET_SINGLE(ObjectManager)->GetTerrainHeight(information);
	// 터레인에 벗어났을 경우
	if (height == -1.f)
	{
		// 부드러운 이동 불가, 수정 필요
		information.position = prevPos;
	}
	else
	{
		// 터레인 통과 불가
		if (information.position.y < height)
			information.position.y = height;
	}
}

void Player::KeyCheck()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.f, 1.f, 0.f);

	// 직진 이동
	if (PUSH_KEY(DIK_W))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position += information.look * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 후진 이동
	if (PUSH_KEY(DIK_S))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position -= information.look * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 좌측 이동
	if (PUSH_KEY(DIK_A))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position -= vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 우측 이동
	if (PUSH_KEY(DIK_D))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position += vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
}

void Player::Render(void)
{
	GameObject::Render();
	/*TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"Look : %d %d %d",
		(int)information.look.x, (int)information.look.y, (int)information.look.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);*/

	// 해당 오브젝트 회전 실험
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	information.mesh->DrawSubset(0);
}

void Player::Release(void)
{
}

Player::Player()
{
}

Player::Player(const TCHAR * _key, INFO _info)
	:GameObject(_key, _info)
{
	memset(&prevPos,NULL, sizeof(D3DXVECTOR3));
}


Player::~Player()
{
	Release();
}

#include "stdafx.h"
#include "Player.h"

#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	// 생선된 캐릭터 메쉬 복제
	GET_SINGLE(MeshManager)->CloneMesh(L"Player", &information.mesh);
	
	// 충돌 정점 설정
	D3DXVECTOR3* vertices;
	information.mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertices);
	D3DXComputeBoundingBox(vertices, information.mesh->GetNumVertices(),
							D3DXGetFVFVertexSize(information.mesh->GetFVF()), 
							&information.min, &information.max);
	information.mesh->UnlockVertexBuffer();

	// 매트릭스 초기화
	D3DXMatrixIdentity(&information.world);

	return S_OK;
}

void Player::Progress(void)
{
	if (GET_SINGLE(DXInput)->PushRight())
		KeyCheck();

	// 이동했을 시 충돌 체크
}

void Player::KeyCheck()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.f, 1.f, 0.f);

	// 직진 이동
	if (PUSH_KEY(DIK_W))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position += information.look * GET_SINGLE(TimeManager)->GetTime();
	}
	// 후진 이동
	if (PUSH_KEY(DIK_S))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position -= information.look * GET_SINGLE(TimeManager)->GetTime();
	}
	// 좌측 이동
	if (PUSH_KEY(DIK_A))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position -= vRight * GET_SINGLE(TimeManager)->GetTime();
	}
	// 우측 이동
	if (PUSH_KEY(DIK_D))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position += vRight * GET_SINGLE(TimeManager)->GetTime();
	}
}

void Player::Render(void)
{
	GameObject::Render();
	/*TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"Pos : %d %d %d",
		(int)information.position.x, (int)information.position.y, (int)information.position.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);*/
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

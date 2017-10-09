#include "stdafx.h"
#include "Player.h"

#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	// ������ ĳ���� �޽� ����
	GET_SINGLE(MeshManager)->CloneMesh(L"Player", &information.mesh);
	
	// �浹 ���� ����
	information.min = *GET_SINGLE(MeshManager)->GetMin(L"Player");
	information.max = *GET_SINGLE(MeshManager)->GetMax(L"Player");

	// ��Ʈ���� �ʱ�ȭ
	D3DXMatrixIdentity(&information.world);

	// �÷��̾� �ӵ� �ʱ�ȭ
	information.speed = 10.f;

	// ���� ���� �ʱ�ȭ
	flying = true;
	
	// ���� �ӵ� �ʱ�ȭ
	fallingSpeed = 2.0f;

	// �ӽ� : boost ���� ������ �ʱ�ȭ

	boostspeed = 0.0f;
	bospeedmax = 2.4f;
	bospeedincrese = 0.1f;
	bospeeddecrese = 0.3f;

	burstspeed = 0.0f;
	buspeedmax = 20.0f;
	buspeeddecrese = 3.0f;



	return S_OK;
}

void Player::Progress(void)
{
	prevPos = information.position;
	//if (GET_SINGLE(DXInput)->PushRight())
	KeyCheck();
	SetPosition();
}

void Player::SetPosition()
{
	float height = GET_SINGLE(ObjectManager)->GetTerrainHeight(information);
	// �ͷ��ο� ����� ���
	if (height == -1.f)
	{
		// �ε巯�� �̵� �Ұ�, ���� �ʿ�
		information.position = prevPos;
	}
	else
	{
		// �ͷ��� ��� �Ұ�
		if (information.position.y < height)
			information.position.y = height;
	}
}

void Player::KeyCheck()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.f, 0.f, 0.f);

	// ���� �̵�
	if (PUSH_KEY(DIK_W))
	{
		move = information.look;

		move.y = 0.0f;
		
		D3DXVec3Normalize(&move, &move);
		information.position += move * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// ���� �̵�
	if (PUSH_KEY(DIK_S))
	{
		move = information.look;

		move.y = 0.0f;

		D3DXVec3Normalize(&move, &move);
		information.position -= move * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// ���� �̵�
	if (PUSH_KEY(DIK_A))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position -= vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// ���� �̵�
	if (PUSH_KEY(DIK_D))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position += vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}

	


	//falling
	D3DXVECTOR3 failing;
	failing.x = 0.0f;
	failing.y = fallingSpeed * -1.0f;
	failing.z = 0.0f;


	information.position += failing * GET_SINGLE(TimeManager)->GetTime() * information.speed;

	//fallingSpeed

	if (PUSH_KEY(DIK_1)) //boost
	{
	
		boostspeed = boostspeed + bospeedincrese;

		if (boostspeed > bospeedmax)
			boostspeed = bospeedmax;
	}
	else
	{
		boostspeed = boostspeed - bospeeddecrese;

		if (boostspeed < 0.0f)
			boostspeed = 0.0f;
	}

	D3DXVECTOR3 boost;
	boost.x = 0.0f;
	boost.y = boostspeed;
	boost.z= 0.0f;

	information.position += boost * GET_SINGLE(TimeManager)->GetTime() * information.speed;

	/*

	burstspeed = 0.0f;
	buspeedmax = 10.0f;
	buspeeddecrese = 3.0f;
	*/

	if (PUSH_KEY(DIK_2)) //burst
	{
		if (burstspeed == 0)
		{
			burstspeed = buspeedmax;
		}
		else
		{
			burstspeed = burstspeed - buspeeddecrese;
		}
	}
	else
	{
		burstspeed = burstspeed - buspeeddecrese;
	}

	if (burstspeed < 0.0f)
		burstspeed = 0.0f;


	D3DXVECTOR3 burst;
	burst.x = 0.0f;
	burst.y = burstspeed;
	burst.z = 0.0f;

	information.position += burst * GET_SINGLE(TimeManager)->GetTime() * information.speed;



}

void Player::Render(void)
{
	GameObject::Render();
	/*TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"Look : %d %d %d",
		(int)information.look.x, (int)information.look.y, (int)information.look.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);*/


	GET_SINGLE(MeshManager)->Mesh_Render(L"Player");
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

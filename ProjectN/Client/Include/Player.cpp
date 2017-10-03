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

	// ���� �̵�
	if (PUSH_KEY(DIK_W))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position += information.look * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// ���� �̵�
	if (PUSH_KEY(DIK_S))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position -= information.look * GET_SINGLE(TimeManager)->GetTime() * information.speed;
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
}

void Player::Render(void)
{
	GameObject::Render();
	/*TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"Look : %d %d %d",
		(int)information.look.x, (int)information.look.y, (int)information.look.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);*/

	// �ش� ������Ʈ ȸ�� ����
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

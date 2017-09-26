#include "stdafx.h"
#include "Player.h"

#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	//GET_SINGLE(MeshManager)->CloneMesh(L"Player", &mesh);
	D3DXMatrixIdentity(&information.world);
	cameraSpeed = 500.f;
	return S_OK;
}

void Player::Progress(void)
{
	if (GET_SINGLE(DXInput)->PushRight())
	{
		KeyCheck();
	}
}

void Player::KeyCheck()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.f, 1.f, 0.f);
	if (GET_SINGLE(DXInput)->GetMouseState2().lX)
	{
		D3DXMATRIX matAxis;
		D3DXMatrixRotationAxis(&matAxis, &up,
			D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lX
				* cameraSpeed
				* GET_SINGLE(TimeManager)->GetTime()));
		D3DXVec3TransformNormal(&information.look, &information.look, &matAxis);
	}
	if (GET_SINGLE(DXInput)->GetMouseState2().lY)
	{
		D3DXVECTOR3 prevLook = information.look;
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		D3DXMATRIX	matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lY
			* cameraSpeed
			* GET_SINGLE(TimeManager)->GetTime()));
		D3DXVec3TransformNormal(&information.look, &information.look, &matAxis);

		if (information.look.y < -0.9f && information.look.z < 0.1f)
			information.look = prevLook;
		if (information.look.y > 0.9f && information.look.z < 0.1f)
			information.look = prevLook;
	}
	if (PUSH_KEY(DIK_W))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position += information.look * GET_SINGLE(TimeManager)->GetTime();
	}
	if (PUSH_KEY(DIK_S))
	{
		D3DXVec3Normalize(&information.look, &information.look);
		information.position -= information.look * GET_SINGLE(TimeManager)->GetTime();
	}
	if (PUSH_KEY(DIK_A))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position -= vRight * GET_SINGLE(TimeManager)->GetTime();

	}
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
	TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"Pos : %d %d %d",
		(int)information.position.x, (int)information.position.y, (int)information.position.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);
	//GET_SINGLE(MeshManager)->Mesh_Render(L"Player");

	/*system("cls");
	cout << "(Pos) x : " << information.position.x
		<< " y : " << information.position.y
		<< " z : " << information.position.z << endl;
	cout << "(Look) x : " << information.look.x
		<< " y : " << information.look.y
		<< " z : " << information.look.z << endl;*/
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
}


Player::~Player()
{
	Release();
}

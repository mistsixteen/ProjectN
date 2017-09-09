#include "stdafx.h"
#include "Player.h"

#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	//GET_SINGLE(MeshManager)->CloneMesh(L"Player", &mesh);
	D3DXMatrixIdentity(&information.world);
	information.look = information.direction - information.position;
	return S_OK;
}

void Player::Progress(void)
{
	if (GET_SINGLE(DXInput)->PushRight())
	{
		if (GET_SINGLE(DXInput)->GetMouseState2().lX)
		{
			information.look = information.direction - information.position;
			
			D3DXMATRIX axis;
			D3DXMatrixRotationAxis(&axis, new D3DXVECTOR3(0.f, 1.f, 0.f), 
									D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lX 
									* 100.f * GET_SINGLE(TimeManager)->GetTime()));
			D3DXVec3TransformNormal(&information.look, &information.look, &axis);
		}
		if (GET_SINGLE(DXInput)->GetMouseState2().lY)
		{

		}
	}
}

void Player::Render(void)
{
	GameObject::Render();
	TCHAR strTmp[128] = L"";
	wsprintf(strTmp, L"dir : %d %d %d", information.direction.x, information.direction.y, information.direction.z);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);
	//GET_SINGLE(MeshManager)->Mesh_Render(L"Player");
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

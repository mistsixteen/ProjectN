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
	printf("%f %f %f", information.position.x, information.position.y, information.position.z);
	GameObject::Render();
	TCHAR strTmp[128] = L"";
	//wsprintf(strTmp, L"pos : %f %f %f", information.position.x, information.position.y, information.position.z);
	
	_stprintf_s(strTmp, _countof(strTmp), L"%f", information.position.y);
	GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);
	//GET_SINGLE(MeshManager)->Mesh_Render(L"Player");
	D3DXVECTOR3 tempPos = information.position;
	
	BOOL isMovable = true;
	tempPos.y = tempPos.y - 0.001f;

	GET_SINGLE(ObjectManager)->ColisionCheck(this, &tempPos, &isMovable);

	if (isMovable == true)
		information.position = tempPos;

	
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

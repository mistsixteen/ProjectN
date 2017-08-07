#include "stdafx.h"
#include "Player.h"

HRESULT Player::Initialize(void)
{
	GET_SINGLE(MeshManager)->CloneMesh(L"Box", &mesh);
	D3DXMatrixIdentity(&information.world);
	return S_OK;
}

void Player::Progress(void)
{
	D3DXMATRIX scale, rotX, rotY, rotZ, trans;

	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&rotX, information.direction.x);
	D3DXMatrixRotationY(&rotY, information.direction.y);
	D3DXMatrixRotationZ(&rotZ, information.direction.z);
	D3DXMatrixTranslation(&trans, information.position.x, information.position.y, information.position.z);
	information.world = scale * rotX * rotY * rotZ * trans;

	device->SetTransform(D3DTS_WORLD, &information.world);
}

void Player::Render(void)
{
	GET_SINGLE(MeshManager)->Mesh_Render(L"Box");
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

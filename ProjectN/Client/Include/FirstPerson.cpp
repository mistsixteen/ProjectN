#include "stdafx.h"
#include "FirstPerson.h"

void FirstPerson::Initialize()
{
	eye = D3DXVECTOR3(5.f, 10.f, -15.f);
	at = D3DXVECTOR3(0.f, 0.f, 0.f);
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(60.f), WINSIZEX / WINSIZEY, 1.0f, 1000.f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void FirstPerson::Progress()
{
	// 플레이어 기준으로 설정
}

FirstPerson::FirstPerson()
{
}


FirstPerson::~FirstPerson()
{
}

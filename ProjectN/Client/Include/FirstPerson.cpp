#include "stdafx.h"
#include "FirstPerson.h"

void FirstPerson::Initialize()
{
	targetInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	eye = targetInfo.position;
	at = eye + targetInfo.look;
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(60.f), WINSIZEX / WINSIZEY, 1.0f, 1000.f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

void FirstPerson::Progress()
{
	// 플레이어 기준으로 설정
	if (GET_SINGLE(DXInput)->PushRight())
	{
		RECT windowRect;
		GetWindowRect(g_hWnd, &windowRect);
		ClipCursor(&windowRect);

	}
	else
		ClipCursor(new RECT);

	targetInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	eye = targetInfo.position;
	at = eye + targetInfo.look;

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	device->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(60.f), WINSIZEX / WINSIZEY, 1.0f, 1000.f);
	device->SetTransform(D3DTS_PROJECTION, &projectionMatrix);
}

FirstPerson::FirstPerson()
{
}


FirstPerson::~FirstPerson()
{
}

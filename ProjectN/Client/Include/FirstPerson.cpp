#include "stdafx.h"
#include "FirstPerson.h"

void FirstPerson::Initialize()
{
	targetInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	eye = targetInfo.position;
	at = targetInfo.look + eye;
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(60.f), WINSIZEX / (float)WINSIZEY, 0.f, 1000.f);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	/* �ȼ�ȭ�� �ؽ��ĸ� �������� �÷� ����ȭ */
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	/* ���� �ѱ� */
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
}

void FirstPerson::Progress()
{
	// �÷��̾� �������� ����
	if (GET_SINGLE(DXInput)->PushRight())
	{
		RECT windowRect;
		GetWindowRect(g_hWnd, &windowRect);
		ClipCursor(&windowRect);
	}
	else
		ClipCursor(NULL);

	targetInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	eye = targetInfo.position;
	at = eye + targetInfo.look;

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);
}

FirstPerson::FirstPerson()
{
}


FirstPerson::~FirstPerson()
{
}

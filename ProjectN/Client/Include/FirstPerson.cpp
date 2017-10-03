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

		INFO& information = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();

		// ī�޶� �¿�
		if (GET_SINGLE(DXInput)->GetMouseState2().lX)
		{
			D3DXMATRIX matAxis;
			D3DXMatrixRotationAxis(&matAxis, &up,
				D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lX
					* cameraSpeed
					* GET_SINGLE(TimeManager)->GetTime()));
			D3DXVec3TransformNormal(&information.look, &information.look, &matAxis);

			// �¿� ��ȯ�� ���� ������Ʈ ���� �ʿ�
		}
		
		// ī�޶� ����
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
	cameraSpeed = 1000.f;
}


FirstPerson::~FirstPerson()
{
}

#include "stdafx.h"
#include "BackView.h"

/* 오브젝트 정렬용 전역 카메라 변수 */
D3DXVECTOR3 gCameraEye;

void BackView::Initialize()
{
	INFO& information = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();

	eye = information.position + information.look * -5.f;
	eye.y += 1.f;
	at = information.position + D3DXVECTOR3(0.f, 1.f, 0.f);
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DXToRadian(60.f), WINSIZEX / (float)WINSIZEY, 1.f, 1000.f);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	/* 픽셀화된 텍스쳐를 선형으로 늘려 선명화 */
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void BackView::Progress()
{
	INFO& information = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();

	at = information.position + D3DXVECTOR3(0.f, 1.f, 0.f);
	if (GET_SINGLE(DXInput)->PushRight())
	{
		RECT windowRect;
		GetWindowRect(g_hWnd, &windowRect);
		ClipCursor(&windowRect);

		// 카메라 좌우
		if (GET_SINGLE(DXInput)->GetMouseState2().lX)
		{
			D3DXMATRIX matAxis;
			D3DXMatrixRotationAxis(&matAxis, &up,
				D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lX
					* cameraSpeed
					* GET_SINGLE(TimeManager)->GetTime()));
			D3DXVec3TransformNormal(&information.look, &information.look, &matAxis);

			// 좌우 전환에 따른 오브젝트 설정 필요
			information.direction.y += D3DXToRadian(GET_SINGLE(DXInput)->GetMouseState2().lX
				* cameraSpeed
				* GET_SINGLE(TimeManager)->GetTime());
		}

		// 카메라 상하
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

	
	eye = information.position + information.look * -5.f;
	eye.y += 1.f;
	at = information.position + D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);

	/* 전역 카메라 위치 변수 초기화 */
	gCameraEye = eye;
}

BackView::BackView()
{
	cameraSpeed = 1000.f;
}


BackView::~BackView()
{
}

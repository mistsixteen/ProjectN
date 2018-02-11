#include "stdafx.h"
#include "BackView.h"

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
	INFO& playerInfo = GET_SINGLE(ObjectManager)->GetGameObject(L"Player")->GetInfo();
	LONG mouseX = GET_SINGLE(DXInput)->GetMouseState2().lX;
	LONG mouseY = GET_SINGLE(DXInput)->GetMouseState2().lY;

	if (GET_SINGLE(DXInput)->PushRight())
	{
		RECT windowRect;
		GetWindowRect(g_hWnd, &windowRect);
		ClipCursor(&windowRect);

		// 카메라 좌우
		if (mouseX)
		{
			float angleY = mouseX * cameraSpeed;
			D3DXMATRIX matAxis;
			D3DXMatrixRotationAxis(&matAxis, &up, D3DXToRadian(angleY));
			D3DXVec3TransformNormal(&playerInfo.look, &playerInfo.look, &matAxis);

			// 좌우 전환에 따른 오브젝트 설정 필요
			playerInfo.direction.y += D3DXToRadian(angleY);
		}


		// 카메라 상하
		if (mouseY)
		{
			D3DXVECTOR3 prevLook = playerInfo.look;
			float angleZ = mouseY * cameraSpeed;

			D3DXVECTOR3 vRight;
			D3DXVec3Cross(&vRight, &up, &playerInfo.look);
			D3DXVec3Normalize(&vRight, &vRight);

			D3DXMATRIX	matAxis;
			D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(angleZ));
			D3DXVec3TransformNormal(&playerInfo.look, &playerInfo.look, &matAxis);

			if (playerInfo.look.y < -0.8f)
				playerInfo.look = prevLook;
			if (playerInfo.look.y > 0.8f)
				playerInfo.look = prevLook;
		}
	}
	else
		ClipCursor(NULL);

	eye = playerInfo.position + playerInfo.look * -5.f;
	eye.y += 1.f;
	at = playerInfo.position + D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_VIEW, &viewMatrix);
}

BackView::BackView()
{
	cameraSpeed = 1.f;
}


BackView::~BackView()
{
}

#include "stdafx.h"
#include "FirstPerson.h"

void FirstPerson::Initialize()
{
	// 플레이어를 기준으로 설정
	eye = D3DXVECTOR3(10.f, 10.f, -10.f);
	at = D3DXVECTOR3(0.f, 0.f, 0.f);
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	// 뷰 행렬
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_VIEW, &viewMatrix);

	// 투영 행렬
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3.f /* 60도 */, WINSIZEX / WINSIZEY, 1.f, 1000.f);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	// 픽셀화된 텍스쳐를 선형으로 늘려 선명하게 함
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		
	// 조명 켜기
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
}

void FirstPerson::Progress()
{
	// 플레이어 기준으로 설정

	// 뷰 행렬
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_VIEW, &viewMatrix);
}

FirstPerson::FirstPerson()
{
}


FirstPerson::~FirstPerson()
{
}

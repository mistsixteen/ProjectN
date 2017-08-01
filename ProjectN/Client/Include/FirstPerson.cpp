#include "stdafx.h"
#include "FirstPerson.h"

void FirstPerson::Initialize()
{
	// �÷��̾ �������� ����
	eye = D3DXVECTOR3(10.f, 10.f, -10.f);
	at = D3DXVECTOR3(0.f, 0.f, 0.f);
	up = D3DXVECTOR3(0.f, 1.f, 0.f);

	// �� ���
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_VIEW, &viewMatrix);

	// ���� ���
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3.f /* 60�� */, WINSIZEX / WINSIZEY, 1.f, 1000.f);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	// �ȼ�ȭ�� �ؽ��ĸ� �������� �÷� �����ϰ� ��
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GET_SINGLE(DXFramework)->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		
	// ���� �ѱ�
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
}

void FirstPerson::Progress()
{
	// �÷��̾� �������� ����

	// �� ���
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
	GET_SINGLE(DXFramework)->SetTransform(D3DTS_VIEW, &viewMatrix);
}

FirstPerson::FirstPerson()
{
}


FirstPerson::~FirstPerson()
{
}

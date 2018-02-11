#include "stdafx.h"
#include "CameraManager.h"

void CameraManager::InitCamera(CAMERA type, const TCHAR * objectKey)
{
	SAFE_DELETE(camera);
	switch (type)
	{
	case CAMERA_FIRST_PERSON:
		camera = new FirstPerson;
		break;
	case CAMERA_BACKVIEW:
		camera = new BackView;
		break;
	}

	camera->Initialize();
}

void CameraManager::Progress()
{
	camera->Progress();
}

void CameraManager::Release()
{
	SAFE_DELETE(camera);
}

HRESULT CameraManager::SetLight()
{
	ZeroMemory(&lightMaterial, sizeof(lightMaterial));
	lightMaterial.Diffuse.r = lightMaterial.Ambient.r = 1.f;
	lightMaterial.Diffuse.g = lightMaterial.Ambient.g = 1.f;
	lightMaterial.Diffuse.b = lightMaterial.Ambient.b = 1.f;
	lightMaterial.Diffuse.a = lightMaterial.Ambient.a = 1.f;
	GET_SINGLE(DXFramework)->GetDevice()->SetMaterial(&lightMaterial);

	// White directional light ���� �� ���� ����
	// �Ѳ����� ���� light���� ���� �� ����
	// ����Ʈ�� �߰��� ���� �ӵ��� ������
	// �������� �����ϵ��� �Ϸ��� D3DRS_LIGHTING ���� ���¸� �����ؾ� ��
	
	D3DXVECTOR3	vDir;
	D3DLIGHT9	light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vDir = D3DXVECTOR3( cosf(GET_SINGLE(TimeManager)->GetTime() / 350.0f), 1.0f, sinf(GET_SINGLE(TimeManager)->GetTime() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vDir);
	light.Range = 1000.0f;

	GET_SINGLE(DXFramework)->GetDevice()->SetLight(0, &light);							// ����Ʈ ��ȣ�� ���� ����Ʈ ����
	GET_SINGLE(DXFramework)->GetDevice()->LightEnable(0, TRUE);							// ����Ʈ ��ȣ�� �̿��� �ش� ����Ʈ �ѱ�
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00202020);	// ���������� �ں��Ʈ �ѱ�

	return S_OK;
}

CameraManager::CameraManager()
	:camera(NULL)
{
}


CameraManager::~CameraManager()
{
	Release();
}

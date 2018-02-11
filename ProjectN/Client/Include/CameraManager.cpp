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

	// White directional light 설정 및 방향 지정
	// 한꺼번에 많은 light들이 사용될 수 있음
	// 라이트가 추가될 수록 속도는 느려짐
	// 라이팅이 가능하도록 하려면 D3DRS_LIGHTING 렌더 상태를 설정해야 함
	
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

	GET_SINGLE(DXFramework)->GetDevice()->SetLight(0, &light);							// 라이트 번호에 따른 라이트 설정
	GET_SINGLE(DXFramework)->GetDevice()->LightEnable(0, TRUE);							// 라이트 번호를 이용한 해당 라이트 켜기
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0x00202020);	// 최종적으로 앰비언트 켜기

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

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

CameraManager::CameraManager()
{
}


CameraManager::~CameraManager()
{
	Release();
}

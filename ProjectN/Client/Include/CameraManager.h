#pragma once
#include "Define.h"
#include "FirstPerson.h"

class CameraManager
{
	DECLARE_SINGLETON(CameraManager);
private:
	Camera*			camera;
public:
	Camera* GetCameraInfo() const { return camera; }
public:
	void InitCamera(CAMERA type, const TCHAR* objectKey = NULL);
	void Progress();
	void Release();
public:
	D3DXVECTOR3 GetEye() const { return camera->GetEye(); }
	D3DXVECTOR3 GetAt() const { return camera->GetAt(); }
private:
	CameraManager();
public:
	~CameraManager();
};


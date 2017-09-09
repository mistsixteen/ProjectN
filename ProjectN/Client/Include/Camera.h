#pragma once
#include "DXFramework.h"
#include "Input.h"
#include "ObjectManager.h"

class Camera
{
protected:
	INFO			targetInfo;
protected:
	D3DXVECTOR3		eye;
	D3DXVECTOR3		at;
	D3DXVECTOR3		up;
protected:
	D3DXMATRIX		viewMatrix;
	D3DXMATRIX		projectionMatrix;
	D3DXMATRIX		worldMatrix;
public:
	virtual void Initialize()PURE;
	virtual void Progress() PURE;
public:
	D3DXVECTOR3 GetEye() const { return eye; }
	D3DXVECTOR3 GetAt() const { return at; }
public:
	Camera();
	virtual ~Camera();
};


#pragma once
#include "DXFramework.h"

class Camera
{
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


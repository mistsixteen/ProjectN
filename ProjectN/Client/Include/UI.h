#pragma once

#include "ObjectManager.h"
#include "TimeManager.h"
#include "TextureManager.h"

class UI
{
protected:
	D3DXMATRIX				world;
	D3DXMATRIX				scale;
	D3DXMATRIX				rotX;
	D3DXMATRIX				rotY;
	D3DXMATRIX				rotZ;
	D3DXMATRIX				trans;
protected:
	INFO					information;
	const TCHAR*			key;
protected:
	D3DXMATRIX				projection;
	D3DXMATRIX				view;
	D3DXVECTOR3				converPos;
	float					sizeX, sizeY;
	RECT					rc;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void) PURE;
	virtual	void	Release(void) PURE;
public:
	UI();
	UI(const TCHAR* _key);
	virtual ~UI();
};


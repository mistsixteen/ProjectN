#pragma once
#include "Define.h"
#include "DXFramework.h"

class GameObject
{
protected:
	INFO					information;
	const TCHAR*			key;
protected:
	LPD3DXMESH				mesh;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void) PURE;
	virtual	void	Release(void) PURE;
public:
	GameObject();
	GameObject(const TCHAR* _key);
	virtual ~GameObject();
};


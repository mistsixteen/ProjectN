#pragma once
#include "Define.h"

#include "ObjectManager.h"
#include "MeshManager.h"

class GameObject
{
protected:
	LPD3DXMESH				mesh;
protected:
	INFO					information;
	const TCHAR*			key;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void) PURE;
	virtual	void	Release(void) PURE;
public:
	INFO& GetInfo() { return information; }
public:
	GameObject();
	GameObject(const TCHAR* _key, INFO _info);
	virtual ~GameObject();
};


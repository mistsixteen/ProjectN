#pragma once
#include "Define.h"

#include "Math.h"

#include "ObjectManager.h"
#include "MeshManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"

class GameObject
{
protected:
	INFO					information;
	const TCHAR*			key;

	VTXTEX*					vtxTex;
	VTXCOL*					vtxCol;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void);
	virtual	void	Release(void) PURE;
public:
	INFO& GetInfo() { return information; }
	VTXTEX* GetVtxTex() const { return vtxTex; }
	VTXCOL* GetVtxCol() const { return vtxCol; }
public:
	GameObject();
	GameObject(const TCHAR* _key, INFO _info);
	virtual ~GameObject();
};


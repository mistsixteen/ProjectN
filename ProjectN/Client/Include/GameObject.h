#pragma once

#include "ObjectManager.h"
#include "MeshManager.h"
#include "BufferManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

class GameObject
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

	VTXTEX*					vtxTex;
	VTXCOL*					vtxCol;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void);
	virtual	void	Release(void) PURE;
	virtual	void	Oncolide(void) PURE;
public:
	INFO& GetInfo() { return information; }
	VTXTEX* GetVtxTex() const { return vtxTex; }
	VTXCOL* GetVtxCol() const { return vtxCol; }
public:
	GameObject();
	GameObject(const TCHAR* _key, INFO _info);
	virtual ~GameObject();
};


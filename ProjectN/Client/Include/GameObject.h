#pragma once
#include "Define.h"
#include "DXFramework.h"

class GameObject
{
protected:
	INFO					information;
	const TCHAR*			key;
protected:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	int						vertexSize;
	int						vertexCount;
	DWORD					vertexFVF;
protected:
	LPDIRECT3DINDEXBUFFER9	indexBuffer;
	int						triangleCount;
	int						indexSize;
	D3DFORMAT				indexFormat;
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void);
	virtual	void	Release(void);
public:
	GameObject();
	GameObject(const TCHAR* _key);
	virtual ~GameObject();
};


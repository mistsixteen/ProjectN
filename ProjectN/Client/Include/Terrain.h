#pragma once
#include "GameObject.h"

class Terrain :
	public GameObject
{
public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();
	virtual	void	Oncolide(void);
public:
	Terrain();
	Terrain(const TCHAR* _key, INFO _info);
	~Terrain();
};


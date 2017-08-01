#pragma once
#include "GameObject.h"

class Terrain :
	public GameObject
{
private:
	int						vertexCountX;
	int						vertexCountZ;
	int						vertexGap;
private:

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();
public:
	Terrain();
	Terrain(const TCHAR* _key, const int _vertexCountX, const int _vertexCountZ, const int _vertexGap);
	~Terrain();
};


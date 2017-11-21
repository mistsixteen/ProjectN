#pragma once
#include "Mesh.h"
class SphereMesh :
	public Mesh
{
public:
	virtual HRESULT Initialize(const TCHAR* path = NULL, const TCHAR* fileName = NULL);
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh);
	virtual void Render();
public:
	SphereMesh();
	~SphereMesh();
};


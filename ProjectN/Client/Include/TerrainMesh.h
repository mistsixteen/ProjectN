#pragma once
#include "Mesh.h"
class TerrainMesh :
	public Mesh
{
public:
	virtual HRESULT Initialize(const TCHAR* path = NULL, const TCHAR* fileName = NULL);
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh);
	virtual void Mesh_Render();
public:
	TerrainMesh();
	~TerrainMesh();
};


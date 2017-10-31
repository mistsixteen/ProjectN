#pragma once
#include "Mesh.h"

class TerrainMesh :
	public Mesh
{
private:
	int			vtxCnt;
	int			idxCnt;
public:
	virtual HRESULT Initialize(const TCHAR* path = NULL, const TCHAR* fileName = NULL);
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh);
	virtual void Render();
public:
	TerrainMesh();
	~TerrainMesh();
};


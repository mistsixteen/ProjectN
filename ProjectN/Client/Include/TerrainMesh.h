#pragma once
#include "Mesh.h"

class TerrainMesh :
	public Mesh
{
private:
	int			vtxCnt;
	int			idxCnt;
public:
	virtual HRESULT Initialize(const int vtxCntX, const int vtxCntZ, const int vtxGap);
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh);
	virtual void Mesh_Render();
public:
	TerrainMesh();
	~TerrainMesh();
};


#pragma once

class Mesh
{
protected:
	LPD3DXMESH	mesh;
	D3DXVECTOR3	min;
	D3DXVECTOR3	max;
public :
	const D3DXVECTOR3* GetMin(void) { return &min; }
	const D3DXVECTOR3*	GetMax(void) { return &max; }
public:
	virtual HRESULT Initialize(const TCHAR* path = NULL, 
								const TCHAR* fileName = NULL) PURE;
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh) { return S_OK; };
	virtual void Mesh_Render() PURE;
public:
	void Release();
public:
	Mesh();
	virtual ~Mesh();
};


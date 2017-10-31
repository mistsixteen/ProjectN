#pragma once
#include "DXFramework.h"

const float sizeX = WINSIZEY / (float)WINSIZEX;

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
	/* 오브젝트 메쉬 전용 초기화 */
	virtual HRESULT Initialize(const TCHAR* path = NULL, const TCHAR* fileName = NULL) PURE;
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh) PURE;
	virtual void Render() PURE;
public:
	void CopyVertexTextureInfo(VTXTEX* vtxTex);
	void PasteVertexTextureInfo(VTXTEX* vtxTex);
public:
	void Release();
public:
	Mesh();
	virtual ~Mesh();
};


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
	/* ������Ʈ �޽� ���� �ʱ�ȭ */
	virtual HRESULT Initialize(const TCHAR* path = NULL, const TCHAR* fileName = NULL) { return S_OK; }
	/* �ͷ��� �޽� ���� �ʱ�ȭ */
	virtual HRESULT Initialize(const int vtxCntX, const int vtxCntZ, const float vtxGap) { return S_OK; }
	virtual HRESULT CloneMesh(LPD3DXMESH* ppMesh) PURE;
	virtual void Mesh_Render() PURE;
public:
	void CopyVertexInfo_VTXTEX(VTXTEX* vtxTex);
	void PasteVertexInfo_VTXTEX(VTXTEX* vtxTex);
public:
	void Release();
public:
	Mesh();
	virtual ~Mesh();
};


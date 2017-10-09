#include "stdafx.h"
#include "BoxMesh.h"


HRESULT BoxMesh::Initialize(const TCHAR * path, const TCHAR * fileName)
{
	// �ڽ� �޽� ����
	if (FAILED(D3DXCreateBox(GET_SINGLE(DXFramework)->GetDevice(), 
							1.f, 1.f, 1.f, &mesh, NULL)))
		return E_FAIL;

	// �ּ� �ִ� �浹 ���� ����
	D3DXVECTOR3* vertices;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertices);
	D3DXComputeBoundingBox(vertices, mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&min, &max);
	mesh->UnlockVertexBuffer();

	//void* vertices;
	//mesh->LockVertexBuffer(0, &vertices);
	//
	//int vtxCnt = mesh->GetNumVertices();
	//DWORD fvf = mesh->GetFVF();
	//int vtxSize = D3DXGetFVFVertexSize(fvf);
	//D3DXComputeBoundingBox((D3DXVECTOR3*)vertices, vtxCnt, vtxSize, &min, &max);

	//mesh->UnlockVertexBuffer();

	//D3DXVECTOR4 vertices[] = {
	//	{ D3DXVECTOR3(-1.0f,1.0f,1.0f),	(float)D3DCOLOR_RGBA(255,0,255,255) },
	//	{ D3DXVECTOR3(1.0f,	1.0f,1.0f),	(float)D3DCOLOR_RGBA(0,255,255,255) },
	//	{ D3DXVECTOR3(1.0f,	1.0f,-1.0f),(float)D3DCOLOR_RGBA(0,0,255,255) },
	//	{ D3DXVECTOR3(-1.0f,1.0f,-1.0f),(float)D3DCOLOR_RGBA(255,255,0,255) },

	//	{ D3DXVECTOR3(-1.0f,-1.0f,1.0f),(float)D3DCOLOR_RGBA(255,0,255,255) },
	//	{ D3DXVECTOR3(1.0f,	-1.0f,1.0f),(float)D3DCOLOR_RGBA(0,255,255,255) },
	//	{ D3DXVECTOR3(1.0f,	-1.0f,-1.0f),(float)D3DCOLOR_RGBA(0,0,255,255) },
	//	{ D3DXVECTOR3(-1.0f,-1.0f,-1.0f),(float)D3DCOLOR_RGBA(255,255,0,255) },
	//};

	//WORD indices[] =
	//{
	//	0,1,2, //����
	//	0,2,3, //����
	//	4,6,5, //�Ʒ���
	//	4,7,6, //�Ʒ���
	//	0,3,7, //������
	//	0,7,4, //������
	//	1,5,6, //������
	//	1,6,2, //������
	//	3,2,6, //�ո�
	//	3,6,7, //�ո�
	//	0,4,5, //�޸�
	//	0,5,1, //�޸�
	//};

	//D3DXCreateMesh(idxCnt * 3, vtxCnt, D3DXMESH_MANAGED | D3DXMESH_32BIT,
	//	decl, device, &mesh);

	//LPVOID pData = NULL;
	//mesh->LockVertexBuffer(0, (void**)&pData);
	//memcpy(pData, vertices, sizeof(vertices));
	//mesh->UnlockVertexBuffer();

	//mesh->LockIndexBuffer(0, (void**)&pData);
	//memcpy(pData, indices, sizeof(indices));
	//mesh->UnlockIndexBuffer();

	return S_OK;
}

HRESULT BoxMesh::CloneMesh(LPD3DXMESH * ppMesh)
{
	D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH];
	mesh->GetDeclaration(decl);
	mesh->CloneMesh(mesh->GetOptions(), decl, 
					GET_SINGLE(DXFramework)->GetDevice(), ppMesh);
	return S_OK;
}

void BoxMesh::Mesh_Render()
{
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DRS_FILLMODE);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mesh->DrawSubset(0);
	GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}

BoxMesh::BoxMesh()
{
}


BoxMesh::~BoxMesh()
{
}

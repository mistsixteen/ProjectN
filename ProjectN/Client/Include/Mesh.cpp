#include "stdafx.h"
#include "Mesh.h"

void Mesh::CopyVertexInfo_VTXTEX(VTXTEX * vtxTex)
{
	LPDIRECT3DVERTEXBUFFER9 vb;
	VTXTEX* oriVtxTex;
	mesh->GetVertexBuffer(&vb);
	vb->Lock(0, 0, (void**)&oriVtxTex, NULL);
	memcpy(vtxTex, oriVtxTex, mesh->GetNumVertices() * sizeof(VTXTEX));
	vb->Unlock();
}

void Mesh::PasteVertexInfo_VTXTEX(VTXTEX * vtxTex)
{
	LPDIRECT3DVERTEXBUFFER9 vb;
	VTXTEX* oriVtxTex;
	mesh->GetVertexBuffer(&vb);
	vb->Lock(0, 0, (void**)&oriVtxTex, NULL);
	memcpy(oriVtxTex, vtxTex, mesh->GetNumVertices() * sizeof(VTXTEX));
	vb->Unlock();
}

void Mesh::Release()
{
	SAFE_RELEASE(mesh);
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	Release();
}

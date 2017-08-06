#include "stdafx.h"
#include "Mesh.h"

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

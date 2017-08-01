#pragma once

// 오브젝트 위치 구조체
typedef struct tInfo
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	D3DXMATRIX		world;
}INFO;

// 오브젝트 버텍스 구조체
typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNormal;
	D3DXVECTOR2 vTexture;
}VTXTEX;
const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// 폴리곤 렌더링 순서 구조체
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;
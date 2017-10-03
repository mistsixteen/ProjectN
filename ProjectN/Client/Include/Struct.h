#pragma once

typedef struct tInfo
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	D3DXMATRIX		world;

	D3DXVECTOR3		min;
	D3DXVECTOR3		max;

	LPD3DXMESH		mesh;

	float			speed;
}INFO;

// 오브젝트 버텍스 구조체
typedef struct tagVertexTexture
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texture;
}VTXTEX;
const DWORD VTXFVF_TEXTURE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// 오브젝트 버텍스 색상 구조체
typedef struct tagVertexColor
{
	D3DXVECTOR3 position;
	DWORD		color;
}VTXCOL;
const DWORD VTXFVF_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE;

// 폴리곤 렌더링 순서 구조체
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;
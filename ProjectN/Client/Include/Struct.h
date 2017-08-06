#pragma once

typedef struct tInfo
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	D3DXMATRIX		world;
}INFO;

// 오브젝트 버텍스 구조체
typedef struct tagVertex
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texture;
}VERTEX;
const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// 폴리곤 렌더링 순서 구조체
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;
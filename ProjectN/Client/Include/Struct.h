#pragma once

typedef struct tInfo
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	D3DXMATRIX		world;
}INFO;

// ������Ʈ ���ؽ� ����ü
typedef struct tagVertex
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texture;
}VERTEX;
const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// ������ ������ ���� ����ü
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;
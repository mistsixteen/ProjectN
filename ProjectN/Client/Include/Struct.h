#pragma once

typedef struct tInfo
{
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;

	D3DXVECTOR3		min;
	D3DXVECTOR3		max;

	LPD3DXMESH		mesh;

	int				hp;
	int				stamina;
	int				damage;

	float			speed;
}INFO;

// ������ ������ ���� ����ü
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;

// ������Ʈ ���ؽ� ���� ����ü
typedef struct tagVertexColor
{
	D3DXVECTOR3 position;
	DWORD		color;
}VTXCOL;
const DWORD VTXFVF_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE;

// ������Ʈ ���ؽ� ����ü
typedef struct tagVertexTexture
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texture;
}VTXTEX;
const DWORD VTXFVF_TEXTURE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// ť�� ���ؽ� ����ü
typedef struct tagVertexCube
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 texture;
}VTXCUBE;
const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

// �ؽ��� ����ü
typedef struct tagTexture
{
	D3DXIMAGE_INFO	information;

	union
	{
		LPDIRECT3DTEXTURE9		texture;
		LPDIRECT3DCUBETEXTURE9	cubeTexture;
	};
}TEXINFO;

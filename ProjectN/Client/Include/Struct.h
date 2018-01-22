#pragma once

#define NONE 0
#define RADIUS 1
#define OBB 2


typedef struct tInfo
{
	// 기본 위치, 오브젝트 방향, 시각 방향
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	
	// 오브젝트 최대, 최소
	D3DXVECTOR3		min;
	D3DXVECTOR3		max;

	// 메쉬
	LPD3DXMESH		mesh;

	// 체력
	int				hp;
	// 스태미나
	int				stamina;
	// 공격력
	int				attack;
	// 피격량
	int				damage;
	// 이동속도
	float			speed;

	//충돌처리

	int				flag;
	float			size;

	D3DXVECTOR3		vAxis[3];
	float			fAxisLen[3];

	tInfo()
	{
		flag = 0;
		size = 0;
	}

}INFO;

// 폴리곤 렌더링 순서 구조체
typedef struct tagIndex
{
	DWORD _1, _2, _3;
}INDEX;

// 오브젝트 버텍스 색상 구조체
typedef struct tagVertexColor
{
	D3DXVECTOR3 position;
	DWORD		color;
}VTXCOL;
const DWORD VTXFVF_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE;

// 오브젝트 버텍스 구조체
typedef struct tagVertexTexture
{
	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texture;
}VTXTEX;
const DWORD VTXFVF_TEXTURE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

// 큐브 버텍스 구조체
typedef struct tagVertexCube
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 texture;
}VTXCUBE;
const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

// 텍스쳐 구조체
typedef struct tagTexture
{
	D3DXIMAGE_INFO	information;

	union
	{
		LPDIRECT3DTEXTURE9		texture;
		LPDIRECT3DCUBETEXTURE9	cubeTexture;
	};
}TEXINFO;

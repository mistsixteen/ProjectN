#pragma once

#include "Factory.h"

#include "ObjectManager.h"
#include "MeshManager.h"
#include "BufferManager.h"
#include "CameraManager.h"

#include "GameObject.h"
#include "Input.h"

class Player :
	public GameObject
{
private:
	D3DXVECTOR3		prevPos;
private:
	bool			flying;
	float			fallingSpeed;

	float			movespeed;

	float			boostspeed;
	float			burstspeed;

	float			bospeedmax;
	float			bospeedincrese;
	float			bospeeddecrese;

	float			buspeedmax;
	float			buspeeddecrese;

	float			temp_firebulletdelay;


public:
	void SetPosition();
	void KeyCheck();
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
	virtual	void	Oncolide(void);
public:
	Player();
	Player(const TCHAR* _key, INFO _info);
	~Player();
};


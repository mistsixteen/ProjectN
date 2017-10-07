#pragma once
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
public:
	void SetPosition();
	void KeyCheck();
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
public:
	Player();
	Player(const TCHAR* _key, INFO _info);
	~Player();
};


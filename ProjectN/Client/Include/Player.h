#pragma once
#include "GameObject.h"
#include "Input.h"

class Player :
	public GameObject
{
private:
	float			cameraSpeed;
public:
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


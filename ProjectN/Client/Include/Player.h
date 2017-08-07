#pragma once
#include "GameObject.h"

class Player :
	public GameObject
{
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


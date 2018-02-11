#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
private:
	bool			isFired;
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
	virtual	void	Oncolide(void);
public:
	Bullet();
	Bullet(const TCHAR* _key, INFO _info);
	~Bullet();
};


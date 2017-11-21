#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
private:
	D3DXVECTOR3		bgMin;
	D3DXVECTOR3		bgMax;
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
private:
	bool isFired() const;
	void initBullet();
public:
	Bullet();
	Bullet(const TCHAR* _key, INFO _info);
	~Bullet();
};


#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
private:
	D3DXVECTOR3		bgMin;
	D3DXVECTOR3		bgMax;

	bool active;


public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
	virtual	void	Oncolide(void);
private:
	bool isFired() const;
	void initBullet();
public:
	Bullet();
	Bullet(const TCHAR* _key, INFO _info);
	~Bullet();
};


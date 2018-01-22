#pragma once
#include "GameObject.h"

class Target :
	public GameObject
{
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
	virtual	void	Oncolide(void);
public:
	Target();
	Target(const TCHAR * _key, INFO _info);
	~Target();
};


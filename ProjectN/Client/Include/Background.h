#pragma once
#include "GameObject.h"

class Background :
	public GameObject
{
public:
	virtual HRESULT Initialize(void);
	virtual	void	Progress(void);
	virtual	void	Render(void);
	virtual	void	Release(void);
	virtual	void	Oncolide(void);
public:
	Background();
	Background(const TCHAR* _key, INFO _info);
	~Background();
};


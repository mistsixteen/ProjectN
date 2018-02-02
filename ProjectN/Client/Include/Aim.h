#pragma once
#include "UI.h"
class Aim :
	public UI
{
	virtual HRESULT Initialize(void);
	virtual void	Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);
public:
	Aim();
	Aim(const TCHAR* _key);
	~Aim();
};


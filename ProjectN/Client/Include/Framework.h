#pragma once
#include "Define.h"

class CFramework
{
public:
	HRESULT InitApp(void);
	void	Progress();
	void	Render();
	void	Release();
public:
	CFramework();
	~CFramework();
};


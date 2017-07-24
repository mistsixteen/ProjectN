#pragma once
#include "Define.h"

class CMainApp
{
public:
	HRESULT InitApp(void);
	void	Progress();
	void	Render();
	void	Release();
public:
	CMainApp();
	~CMainApp();
};


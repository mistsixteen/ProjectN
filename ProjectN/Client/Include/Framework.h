#pragma once
#include "Define.h"

#include "DXFramework.h"
#include "Input.h"

#include "TimeManager.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "MeshManager.h"

class Framework
{
public:
	HRESULT InitApp(void);
	void	Progress();
	void	Render();
	void	Release();
public:
	Framework();
	~Framework();
};


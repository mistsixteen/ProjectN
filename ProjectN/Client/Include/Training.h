#pragma once
#include "Scene.h"

class CTraining :
	public CScene
{
public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();
public:
	CTraining();
	~CTraining();
};


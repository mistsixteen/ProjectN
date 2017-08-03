#pragma once
#include "GameScene.h"
#include "Factory.h"

class Training :
	public GameScene
{
public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();
public:
	Training();
	~Training();
};


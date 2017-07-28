#pragma once
#include "GameObject.h"
#include "ObjectFactory.h"


class GameScene
{
public:
	virtual HRESULT Initialize() PURE;
	virtual void Progress() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
public:
	GameScene();
	virtual ~GameScene();
};


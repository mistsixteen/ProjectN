#pragma once
#include "Factory.h"

#include "ObjectManager.h"
#include "UIManager.h"
#include "MeshManager.h"
#include "BufferManager.h"
#include "CameraManager.h"


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


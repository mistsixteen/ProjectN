#pragma once
#include "Define.h"

#include "ObjMgr.h"

#include "GameScene.h"
#include "Training.h"

class SceneManager
{
	DECLARE_SINGLETON(SceneManager);
private:
	GameScene*	m_pScene;
public:
	HRESULT	InitScene(SCENE_TYPE type);
	void Progress();
	void Render();
	void Release();
private:
	SceneManager();
public:
	~SceneManager();
};


#pragma once
#include "Define.h"

#include "GameObject.h"

#include "GameScene.h"
#include "Training.h"

class SceneManager
{
	DECLARE_SINGLETON(SceneManager);
private:
	GameScene*	scene;
public:
	HRESULT	InitScene(SCENE type);
	void Progress();
	void Render();
	void Release();
private:
	SceneManager();
public:
	~SceneManager();
};


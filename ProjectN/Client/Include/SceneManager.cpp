#include "stdafx.h"
#include "SceneManager.h"

HRESULT SceneManager::InitScene(SCENE type)
{
	SAFE_DELETE(scene);
	switch (type)
	{
	case SCENE_LOGO:
		break;
	case SCENE_TRAINING:
		scene = new Training;
		break;
	}

	if (FAILED(scene->Initialize()))
	{
		MSGBOX(L"SceneManager : 씬 불러오기 실패");
		return E_FAIL;
	}

	return S_OK;
}

void SceneManager::Progress()
{
	scene->Progress();
}

void SceneManager::Render()
{
	scene->Render();
}

void SceneManager::Release()
{
	SAFE_DELETE(scene);
}

SceneManager::SceneManager()
	:scene(NULL)
{
}


SceneManager::~SceneManager()
{
	Release();
}

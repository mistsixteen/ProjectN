#include "stdafx.h"
#include "SceneManager.h"

HRESULT SceneManager::InitScene(SCENE_TYPE type)
{
	SAFE_DELETE(m_pScene);
	switch (type)
	{
	case SCENE_TYPE_LOGO:
		break;
	case SCENE_TYPE_TRAINING:
		m_pScene = new Training;
		break;
	}

	return S_OK;
}

void SceneManager::Progress()
{
}

void SceneManager::Render()
{
}

void SceneManager::Release()
{
}

SceneManager::SceneManager()
	:m_pScene(NULL)
{
}


SceneManager::~SceneManager()
{
	Release();
}

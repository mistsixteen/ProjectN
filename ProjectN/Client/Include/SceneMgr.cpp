#include "stdafx.h"
#include "SceneMgr.h"


HRESULT CSceneMgr::InitState(SCENE_TYPE type)
{
	SAFE_DELETE(m_pScene);
	switch (type)
	{
	case SCENE_TYPE_LOGO:
		break;
	case SCENE_TYPE_TRAINING:
		m_pScene = new CTraining;
		break;
	}

	return S_OK;
}

void CSceneMgr::Progress()
{
}

void CSceneMgr::Render()
{
}

void CSceneMgr::Release()
{
}

CSceneMgr::CSceneMgr()
	:m_pScene(NULL)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

#pragma once
#include "Define.h"

#include "ObjMgr.h"

#include "Training.h"


class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr);
private:
	CScene*	m_pScene;
public:
	HRESULT	InitState(SCENE_TYPE type);
	void Progress();
	void Render();
	void Release();
private:
	CSceneMgr();
public:
	~CSceneMgr();
};


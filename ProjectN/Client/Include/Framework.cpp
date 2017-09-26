#include "stdafx.h"
#include "Framework.h"

Framework::Framework()
{
}

Framework::~Framework()
{
	Release();
}

HRESULT Framework::InitApp(void)
{
	// DirectX 초기화
	if (FAILED(GET_SINGLE(DXFramework)->InitDevice(WINTYPE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSGBOX(L"CMainApp : Failed to initialize device");
		return E_FAIL;
	}

	// DirectX 입력 초기화
	if (FAILED(GET_SINGLE(DXInput)->CreateDevice(g_hWnd)))
	{
		MSGBOX(L"CMainApp : Failed to initialize input device");
		return E_FAIL;
	}

	// 시간 함수 초기화
	GET_SINGLE(TimeManager)->InitTimeMgr();

	// 씬 초기화 (트레이닝 룸으로 초기화)
	GET_SINGLE(SceneManager)->InitScene(SCENE_TRAINING);

	return S_OK;
}

void Framework::Progress()
{
	GET_SINGLE(TimeManager)->SetTime();
	GET_SINGLE(DXInput)->ReadData();
	GET_SINGLE(SceneManager)->Progress();
}

//GET_SINGLE(DXFramework)->Drawtext(szFps, 0, 0);
void Framework::Render()
{
	GET_SINGLE(DXFramework)->Render_Begin();
	
	GET_SINGLE(SceneManager)->Render();

	GET_SINGLE(DXFramework)->Render_End();
}

void Framework::Release()
{
	// 싱글톤 함수 릴리즈
	SAFE_DELETE_SINGLE(DXInput);
	SAFE_DELETE_SINGLE(DXFramework);
	SAFE_DELETE_SINGLE(SceneManager);
	SAFE_DELETE_SINGLE(ObjectManager);
	SAFE_DELETE_SINGLE(TimeManager);
	SAFE_DELETE_SINGLE(CameraManager);
	SAFE_DELETE_SINGLE(MeshManager);
	_CrtDumpMemoryLeaks();
}

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
	// DirectX �ʱ�ȭ
	if (FAILED(GET_SINGLE(DXFramework)->InitDevice(WINTYPE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSGBOX(L"CMainApp : Failed to initialize device");
		return E_FAIL;
	}

	// DirectX �Է� �ʱ�ȭ
	if (FAILED(GET_SINGLE(DXInput)->CreateDevice(g_hWnd)))
	{
		MSGBOX(L"CMainApp : Failed to initialize input device");
		return E_FAIL;
	}

	// �ð� �Լ� �ʱ�ȭ
	GET_SINGLE(TimeManager)->InitTimeMgr();

	// �� �ʱ�ȭ (Ʈ���̴� ������ �ʱ�ȭ)
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
	// �̱��� �Լ� ������
	SAFE_DELETE_SINGLE(DXInput);
	SAFE_DELETE_SINGLE(DXFramework);
	SAFE_DELETE_SINGLE(SceneManager);
	SAFE_DELETE_SINGLE(ObjectManager);
	SAFE_DELETE_SINGLE(TimeManager);
	SAFE_DELETE_SINGLE(CameraManager);
	SAFE_DELETE_SINGLE(MeshManager);
	_CrtDumpMemoryLeaks();
}

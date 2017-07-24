#include "stdafx.h"
#include "MainApp.h"

#include "Device.h"
#include "Input.h"
#include "TimeMgr.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	Release();
}

HRESULT CMainApp::InitApp(void)
{
	// DirectX 초기화
	if (FAILED(GET_SINGLE(CDevice)->InitDevice(WINMODE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSGBOX(L"CMainApp : Failed to initialize device");
		return E_FAIL;
	}

	// DirectX 입력 초기화
	if (FAILED(GET_SINGLE(CInput)->CreateDevice(g_hWnd)))
	{
		MSGBOX(L"CMainApp : Failed to initialize input device");
		return E_FAIL;
	}

	// 시간 함수 초기화
	GET_SINGLE(CTimeMgr)->InitTimeMgr();

	return S_OK;
}

void CMainApp::Progress()
{
	GET_SINGLE(CTimeMgr)->SetTime();
	GET_SINGLE(CInput)->ReadData();
}

void CMainApp::Render()
{
	GET_SINGLE(CDevice)->Render_Begin();

	static int iCnt;
	++iCnt;
	static float fTime;
	fTime += GET_SINGLE(CTimeMgr)->GetTime();

	static TCHAR szFps[128];

	if (fTime > 1.f)
	{
		wsprintf(szFps, L"FPS:%d", iCnt);
		iCnt = 0;
		fTime = 0;
	}
	GET_SINGLE(CDevice)->Drawtext(szFps, 0, 0);

	GET_SINGLE(CDevice)->Render_End();
}

void CMainApp::Release()
{
	SAFE_DELETE_SINGLE(CInput);
	SAFE_DELETE_SINGLE(CTimeMgr);
	SAFE_DELETE_SINGLE(CDevice);
}

#include "stdafx.h"
#include "Framework.h"

#include "Device.h"
#include "Input.h"
#include "TimeMgr.h"

CFramework::CFramework()
{
}

CFramework::~CFramework()
{
	Release();
}

HRESULT CFramework::InitApp(void)
{
	// DirectX 초기화
	if (FAILED(GET_SINGLE(CDevice)->InitDevice(WINTYPE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
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

void CFramework::Progress()
{
	GET_SINGLE(CTimeMgr)->SetTime();
	GET_SINGLE(CInput)->ReadData();
}

void CFramework::Render()
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

void CFramework::Release()
{
	SAFE_DELETE_SINGLE(CInput);
	SAFE_DELETE_SINGLE(CTimeMgr);
	SAFE_DELETE_SINGLE(CDevice);
}

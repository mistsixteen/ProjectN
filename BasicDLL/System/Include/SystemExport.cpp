#include "SystemExport.h"

#include "Device.h"
#include "Input.h"
#include "Time.h"

EXTERN SYSTEM_DLL HRESULT InitDevice(BOOL isFullScreen, HWND hWnd, const int & iWinCX, const int & iWinCY)
{
	return GET_SINGLE(Device)->InitDevice(isFullScreen, hWnd, iWinCX, iWinCY);
}

EXTERN SYSTEM_DLL void Release(void)
{
	GET_SINGLE(Device)->Release();
}

EXTERN SYSTEM_DLL void Render_Begin(void)
{
	GET_SINGLE(Device)->Render_Begin();
}

EXTERN SYSTEM_DLL void Render_End(void)
{
	GET_SINGLE(Device)->Render_End();
}

EXTERN SYSTEM_DLL LPD3DXSPRITE GetSprite()
{
	return GET_SINGLE(Device)->GetSprite();
}

EXTERN SYSTEM_DLL LPDIRECT3DDEVICE9 GetDevice()
{
	return GET_SINGLE(Device)->GetDevice();
}

EXTERN SYSTEM_DLL LPD3DXFONT GetFont()
{
	return GET_SINGLE(Device)->GetFont();
}

EXTERN SYSTEM_DLL LPD3DXLINE GetLine()
{
	return GET_SINGLE(Device)->GetLine();
}

EXTERN SYSTEM_DLL HRESULT InitInput(HWND hWnd)
{
	return GET_SINGLE(Input)->InitInput(hWnd);
}

EXTERN SYSTEM_DLL HRESULT ReadData()
{
	return GET_SINGLE(Input)->ReadData();
}

EXTERN SYSTEM_DLL void OnActivate(WPARAM wParam)
{
	return GET_SINGLE(Input)->OnActivate(wParam);
}

EXTERN SYSTEM_DLL BOOL PushLeft()
{
	return GET_SINGLE(Input)->PushLeft();
}

EXTERN SYSTEM_DLL BOOL PushRight()
{
	return GET_SINGLE(Input)->PushRight();
}

EXTERN SYSTEM_DLL BOOL PushMiddle()
{
	return GET_SINGLE(Input)->PushMiddle();
}

EXTERN SYSTEM_DLL BYTE GetKeyBoardState(short nKey)
{
	return GET_SINGLE(Input)->GetKeyBoardState(nKey);
}

EXTERN SYSTEM_DLL POINT GetptMouse()
{
	return GET_SINGLE(Input)->GetptMouse();
}

EXTERN SYSTEM_DLL DIMOUSESTATE2 GetMouseState2()
{
	return GET_SINGLE(Input)->GetMouseState2();
}

EXTERN SYSTEM_DLL void InitTime()
{
	GET_SINGLE(Time)->InitTime();
}

EXTERN SYSTEM_DLL void SetTime()
{
	GET_SINGLE(Time)->SetTime();
}

EXTERN SYSTEM_DLL float GetTime()
{
	return GET_SINGLE(Time)->GetTime();
}

EXTERN SYSTEM_DLL DWORD GetFps()
{
	return GET_SINGLE(Time)->GetFps();
}

#ifndef _SYSTEMFUNCTION_H_
#define _SYSTEMFUNCTION_H_

#define SYSTEM_EXPORT
#include "SystemDefine.h"

/* Device °´Ã¼ ³» ÇÔ¼ö */
EXTERN SYSTEM_DLL HRESULT InitDevice(BOOL isFullScreen, HWND hWnd, const int& iWinCX, const int& iWinCY);

EXTERN SYSTEM_DLL void Release(void);
EXTERN SYSTEM_DLL void Render_Begin(void);
EXTERN SYSTEM_DLL void Render_End(void);

EXTERN SYSTEM_DLL LPD3DXSPRITE GetSprite();
EXTERN SYSTEM_DLL LPDIRECT3DDEVICE9 GetDevice();
EXTERN SYSTEM_DLL LPD3DXFONT GetFont();
EXTERN SYSTEM_DLL LPD3DXLINE GetLine();

/* Input °´Ã¼ ³» ÇÔ¼ö */
EXTERN SYSTEM_DLL HRESULT InitInput(HWND hWnd);
EXTERN SYSTEM_DLL HRESULT ReadData();
EXTERN SYSTEM_DLL void OnActivate(WPARAM wParam);

EXTERN SYSTEM_DLL BOOL PushLeft();
EXTERN SYSTEM_DLL BOOL PushRight();
EXTERN SYSTEM_DLL BOOL PushMiddle();

EXTERN SYSTEM_DLL BYTE GetKeyBoardState(short nKey);
EXTERN SYSTEM_DLL POINT GetptMouse();
EXTERN SYSTEM_DLL DIMOUSESTATE2 GetMouseState2();

/* Time °´Ã¼ ³» ÇÔ¼ö */
EXTERN SYSTEM_DLL void InitTime();
EXTERN SYSTEM_DLL void SetTime();

EXTERN SYSTEM_DLL float GetTime();
EXTERN SYSTEM_DLL DWORD GetFps();

#endif

#pragma once

class DXInput
{
	DECLARE_SINGLETON(DXInput);
private:
	LPDIRECTINPUT8			m_pDI;
	HWND					m_hWnd;
private:
	LPDIRECTINPUTDEVICE8	m_pMouse;
	DIMOUSESTATE2			m_MouseState2;
	POINT					m_ptMouse;
private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	BYTE					m_byKeyState[256];
public:
	HRESULT CreateDevice(HWND hWnd);
	HRESULT ReadData();
	VOID FreeDirectInput();
	void OnActivate(WPARAM wParam);
public:
	BOOL PushLeft();
	BOOL PushRight();
	BOOL PushMiddle();
public:
	BYTE GetKeyBoardState(short nKey) { return m_byKeyState[nKey]; }
	POINT GetptMouse() const { return m_ptMouse; }
	DIMOUSESTATE2 GetMouseState2() const { return m_MouseState2; }
private:
	DXInput(void);
public:
	~DXInput(void);
};
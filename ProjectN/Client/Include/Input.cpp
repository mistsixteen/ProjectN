#include "stdafx.h"
#include "Input.h"

/*
1단계
*/
HRESULT CInput::CreateDevice(HWND hWnd)
{
	m_hWnd = hWnd;
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
	{
		MSGBOX(L"DirectInput8Create Failed");
		return E_FAIL;
	}

	//==============마우스 설정

	//마우스에 대한 장치 생성
	if (FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		MSGBOX(L"Mouse CreateDevice Failed");
		return E_FAIL;
	}

	//마우스 상태에 대한 데이터 형식 지정
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MSGBOX(L"Mouse SetDataFormat Failed");
		return E_FAIL;
	}

	//DISCL_NONEXCLUSIVE : 다른 장치와 접근 권한을 공유
	//DISCL_FOREGROUND : 활성때에만 장치가 동작
	//마우스 협조 레벨을 얻어온다.
	if (FAILED(m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MSGBOX(L"Mouse SetCooperativeLevel Failed");
		return E_FAIL;
	}

	//응용프로그램으로 마우스의 제어권을 가져오기 위한 함수호출
	m_pMouse->Acquire();

	//==============키보드 설정

	//키보드에 대한 장치 생성
	if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
	{
		MSGBOX(L"Keyboard CreateDevice Failed");
		return E_FAIL;
	}

	//키보드 협조 레벨을 얻어온다.
	if (FAILED(m_pKeyBoard->SetCooperativeLevel(m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		MSGBOX(L"Keyboard CreateDevice Failed");
		return E_FAIL;
	}

	if (FAILED(m_pKeyBoard->SetCooperativeLevel(m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		MSGBOX(L"Keyboard CreateDevice Failed");
		return E_FAIL;
	}
	//키보드 하드웨어 정보 저장
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MSGBOX(L"Keyboard SetDataFormat Failed");
		return E_FAIL;
	}

	//키보드 장치 활성화
	m_pKeyBoard->Acquire();

	return S_OK;
}

/*
2단계
키보드와 동일하게 ReadData()함수를 매 프레임마다 호출하면서
마우스의 이동량을 측정하고 m_ptMouse에 저장
*/
HRESULT CInput::ReadData()
{
	if (NULL == m_pMouse)
		return S_OK;

	ZeroMemory(&m_MouseState2, sizeof(m_MouseState2));
	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState2)))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST)
			m_pMouse->Acquire();
		return S_OK;
	}

	ZeroMemory(&m_byKeyState, sizeof(m_byKeyState));
	if (FAILED(m_pKeyBoard->GetDeviceState(sizeof(m_byKeyState), &m_byKeyState)))
	{
		while (m_pKeyBoard->Acquire() == DIERR_INPUTLOST)
			m_pKeyBoard->Acquire();
		return S_OK;
	}

	m_ptMouse.x = m_MouseState2.lX;
	m_ptMouse.y = m_MouseState2.lY;

	ScreenToClient(m_hWnd, &m_ptMouse);

	return S_OK;
}

/*
3단계
rgbButton 값 중에서 0이면 비클릭 1이면 클릭
*/
BOOL CInput::PushLeft()
{
	return (m_MouseState2.rgbButtons[0] & 0x80) ? TRUE : FALSE;
}

BOOL CInput::PushRight()
{
	return (m_MouseState2.rgbButtons[1] & 0x80) ? TRUE : FALSE;
}

BOOL CInput::PushMiddle()
{
	return (m_MouseState2.rgbButtons[2] & 0x80) ? TRUE : FALSE;
}

/*
4단계
응용프로그램이 활성 또는 비활성 되었을 때
마우스 장치에 대한 제어권을 확보하는 단계
*/
void CInput::OnActivate(WPARAM wParam)
{
	if (WA_INACTIVE != wParam && m_pMouse)
	{
		m_pMouse->Acquire();
	}
}

/*
5단계
장치에 대한 제어권을 먼저 해제한 후에
그 다음 장치를 해제하고
마지막에 DirectInput을 해제
*/
VOID CInput::FreeDirectInput()
{
	SAFE_RELEASE(m_pMouse);
	SAFE_RELEASE(m_pKeyBoard);
	SAFE_RELEASE(m_pDI);
}

CInput::CInput(void)
{
}


CInput::~CInput(void)
{
	FreeDirectInput();
}

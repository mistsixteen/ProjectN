#include "stdafx.h"
#include "Input.h"

/*
1�ܰ�
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

	//==============���콺 ����

	//���콺�� ���� ��ġ ����
	if (FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		MSGBOX(L"Mouse CreateDevice Failed");
		return E_FAIL;
	}

	//���콺 ���¿� ���� ������ ���� ����
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MSGBOX(L"Mouse SetDataFormat Failed");
		return E_FAIL;
	}

	//DISCL_NONEXCLUSIVE : �ٸ� ��ġ�� ���� ������ ����
	//DISCL_FOREGROUND : Ȱ�������� ��ġ�� ����
	//���콺 ���� ������ ���´�.
	if (FAILED(m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		MSGBOX(L"Mouse SetCooperativeLevel Failed");
		return E_FAIL;
	}

	//�������α׷����� ���콺�� ������� �������� ���� �Լ�ȣ��
	m_pMouse->Acquire();

	//==============Ű���� ����

	//Ű���忡 ���� ��ġ ����
	if (FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
	{
		MSGBOX(L"Keyboard CreateDevice Failed");
		return E_FAIL;
	}

	//Ű���� ���� ������ ���´�.
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
	//Ű���� �ϵ���� ���� ����
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MSGBOX(L"Keyboard SetDataFormat Failed");
		return E_FAIL;
	}

	//Ű���� ��ġ Ȱ��ȭ
	m_pKeyBoard->Acquire();

	return S_OK;
}

/*
2�ܰ�
Ű����� �����ϰ� ReadData()�Լ��� �� �����Ӹ��� ȣ���ϸ鼭
���콺�� �̵����� �����ϰ� m_ptMouse�� ����
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
3�ܰ�
rgbButton �� �߿��� 0�̸� ��Ŭ�� 1�̸� Ŭ��
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
4�ܰ�
�������α׷��� Ȱ�� �Ǵ� ��Ȱ�� �Ǿ��� ��
���콺 ��ġ�� ���� ������� Ȯ���ϴ� �ܰ�
*/
void CInput::OnActivate(WPARAM wParam)
{
	if (WA_INACTIVE != wParam && m_pMouse)
	{
		m_pMouse->Acquire();
	}
}

/*
5�ܰ�
��ġ�� ���� ������� ���� ������ �Ŀ�
�� ���� ��ġ�� �����ϰ�
�������� DirectInput�� ����
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

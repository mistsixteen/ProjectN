#include "stdafx.h"
#include "Device.h"

CDevice::CDevice(void)
	:m_p3D(NULL),
	m_pDevice(NULL),
	m_pSprite(NULL),
	m_pFont(NULL),
	m_pLine(NULL)
{
}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice(WINTYPE Mode, HWND hWnd, const int& iWinCX, const int& iWinCY)
{
	//���ø����̼��� �ùٸ� ����� ����������� Ȯ��
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DCAPS9 devicecaps;
	ZeroMemory(&devicecaps, sizeof(devicecaps));

	//D3DADAPTER_DEFAULT �׷���ī�� 2���϶� �� �׷����� �̾ƿ�,
	//D3DDEVTYPE_HAL 3D�� ����� ���� ���ؽ�ó���� �׷��� ī��(�ϵ����)���� �Ѵ�.

	//DEVCAPS �ϵ���� �������� Ȯ��
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&devicecaps)))
	{
		MSGBOX(L"��ġ���� ������ ����");
		return E_FAIL;
	}

	DWORD dwBehaviourFlag = 0;

	//DEVCAPS �ϵ���� �������� Ȯ��
	if (devicecaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)		//�ϵ���� ���ؽ� ���μ��� ���� Ȯ��
		dwBehaviourFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviourFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = iWinCX;
	d3dpp.BackBufferHeight = iWinCY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// �ĸ� ���� �ȼ� ���� ����

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	/* ���� �ҽ��� ���� �κ� */
	d3dpp.hDeviceWindow = hWnd;									//�� ������ �ڵ�
	d3dpp.Windowed = Mode;										//��ü ȭ�� ����

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = true;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//���� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//1.D3DADAPTER_DEFAULT ������ �� DeviceCap�̶� �����ϰ�
	//2.D3DDEVTYPE_HAL ����̽� Ÿ��
	//3.�θ� ������ �ڵ�
	//4.���ؽ� ���μ��� ���
	//5.������ ����ü
	//6.�� ��� LPDIRECT3DDEVICE9 ��ġ�� ���� ������
	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, dwBehaviourFlag, &d3dpp, &m_pDevice)))
	{
		MSGBOX(L"��ġ ���� ����");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		MSGBOX(L"��������Ʈ ���� ����");
		return E_FAIL;
	}

	D3DXFONT_DESC FontInfo;
	ZeroMemory(&FontInfo, sizeof(FontInfo));
	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_HEAVY;
	FontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(FontInfo.FaceName, L"�ü�");
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo, &m_pFont)))
	{
		MSGBOX(L"��Ʈ ���� ����");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		MSGBOX(L"���� ���� ����");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Release()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pLine);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_p3D);
}

void CDevice::Render_Begin()
{
	if (NULL == m_pDevice)
		return;
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
		, D3DCOLOR_ARGB(255, 0, 0, 255), 1.0f, 0);
	m_pDevice->BeginScene();
}

void CDevice::Render_End()
{
	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void CDevice::SetRenderState(_D3DRENDERSTATETYPE Type, DWORD dwFlag)
{
	m_pDevice->SetRenderState(Type, dwFlag);
}

void CDevice::SetTransform(_D3DTRANSFORMSTATETYPE Type, const D3DXMATRIX* pMatrix)
{
	m_pDevice->SetTransform(Type, pMatrix);
}

void CDevice::Drawtext(const TCHAR* Message, long _lX, long _lY, long lSizeX /*= 640*/, long lSizeY /*= 480*/, DWORD Color)
{
	RECT rc = { _lX, _lY, lSizeX, lSizeY };
#ifdef UNICODE
	GetFont()->DrawTextW(0, Message, lstrlen(Message), &rc, DT_TOP | DT_LEFT, Color);
#else
	GetFont()->DrawTextA(0, Message, lstrlen(Message), &rc, DT_TOP | DT_LEFT, Color);
#endif
}

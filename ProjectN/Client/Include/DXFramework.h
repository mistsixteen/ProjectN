#pragma once

class DXFramework
{
	DECLARE_SINGLETON(DXFramework);
private:
	//장치 성능 조사
	LPDIRECT3D9				m_p3D;
	//장치 대표 객체
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXSPRITE			m_pSprite;
	//폰트 출력용
	LPD3DXFONT				m_pFont;
	LPD3DXLINE				m_pLine;
public:
	LPD3DXLINE GetLine() const { return m_pLine; }
	LPD3DXFONT GetFont() const { return m_pFont; }
	LPD3DXSPRITE GetSprite() const { return m_pSprite; }
	LPDIRECT3DDEVICE9 GetDevice() const { return m_pDevice; }
public:
	HRESULT InitDevice(WINTYPE Mode, HWND hWnd, const int& iWinCX, const int& iWinCY);
	void Release();
public:
	void Render_Begin();
	void Render_End();
public:
	void Drawtext(const TCHAR* Message, long _lX, long _lY,
		long lSizeX = 640, long lSizeY = 480, DWORD Color = D3DCOLOR_ARGB(255, 255, 255, 255));
private:
	DXFramework(void);
public:
	~DXFramework(void);
};



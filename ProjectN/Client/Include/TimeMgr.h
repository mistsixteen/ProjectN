#pragma once
#include "Define.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CPUTick;
	float				m_fTime;

	float				m_fFps;
	int					m_iFrameCnt;
	float				m_fPassed;
public:
	void InitTimeMgr();
	void SetTime();
public:
	float GetTime() const { return m_fTime; }
	float GetFps() const { return m_fFps; }
private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};


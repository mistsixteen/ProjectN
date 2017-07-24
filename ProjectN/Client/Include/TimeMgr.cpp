#include "stdafx.h"
#include "TimeMgr.h"

CTimeMgr::CTimeMgr(void)
	:m_fTime(0),
	m_fFps(0.f),
	m_iFrameCnt(0),
	m_fPassed(0.f)
{
}


CTimeMgr::~CTimeMgr(void)
{
}

void CTimeMgr::InitTimeMgr()
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);				//현재 시간
	QueryPerformanceFrequency(&m_CPUTick);				//초당 CPU 연산
}

void CTimeMgr::SetTime()
{
	QueryPerformanceCounter(&m_FrameTime);

	//1초가 지났다는 의미
	if (m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	//FPS 구하기
	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CPUTick.QuadPart;
	m_fPassed += m_fTime;

	++m_iFrameCnt;
	if (m_fPassed > 1.f)
	{
		m_fFps = (float)m_iFrameCnt / m_fPassed;
		m_fPassed = 0.f;
		m_iFrameCnt = 0;
	}

	//m_fTime : 1프레임 당 실행된 시간
	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CPUTick.QuadPart;
	m_FixTime = m_FrameTime;

}
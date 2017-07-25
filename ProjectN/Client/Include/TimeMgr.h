#pragma once
#include "Define.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER		frameTime;
	LARGE_INTEGER		fixTime;
	LARGE_INTEGER		lastTime;
	LARGE_INTEGER		cpuTick;
	float				time;

	float				fps;
	int					frameCount;
	float				passedTime;
public:
	void InitTimeMgr();
	void SetTime();
public:
	float GetTime() const { return time; }
	float GetFps() const { return fps; }
private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};


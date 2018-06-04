#pragma once
#define SYSTEM_EXPORT
#include "SystemDefine.h"

EXTERN class SYSTEM_DLL Time
{
	DECLARE_SINGLETON(Time);
private:
	LARGE_INTEGER		frameTime;
	LARGE_INTEGER		fixTime;
	LARGE_INTEGER		lastTime;
	LARGE_INTEGER		cpuTick;
	float				time;

	DWORD				fps;
	int					frameCount;
	float				passedTime;
public:
	void InitTime();
	void SetTime();
public:
	float GetTime() const { return time; }
	DWORD GetFps() const { return fps; }
private:
	Time(void);
public:
	~Time(void);
};

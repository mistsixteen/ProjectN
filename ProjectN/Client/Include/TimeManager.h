#pragma once

class TimeManager
{
	DECLARE_SINGLETON(TimeManager);
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
	void InitTimeMgr();
	void SetTime();
public:
	float GetTime() const { return time; }
	DWORD GetFps() const { return fps; }
private:
	TimeManager(void);
public:
	~TimeManager(void);
};


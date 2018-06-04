#include "Time.h"



void Time::InitTime()
{
	QueryPerformanceCounter(&frameTime);
	QueryPerformanceCounter(&fixTime);
	QueryPerformanceCounter(&lastTime);				//현재 시간
	QueryPerformanceFrequency(&cpuTick);			//초당 CPU 연산
}

void Time::SetTime()
{
	QueryPerformanceCounter(&frameTime);

	//1초가 지났다는 의미
	if (frameTime.QuadPart - lastTime.QuadPart > cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&cpuTick);
		lastTime.QuadPart = frameTime.QuadPart;
	}

	//FPS 구하기
	time = float(frameTime.QuadPart - fixTime.QuadPart) / cpuTick.QuadPart;
	passedTime += time;

	++frameCount;
	if (passedTime > 1.f)
	{
		fps = DWORD(frameCount / passedTime);
		passedTime = 0.f;
		frameCount = 0;
	}

	//time : 1프레임 당 실행된 시간
	time = float(frameTime.QuadPart - fixTime.QuadPart) / cpuTick.QuadPart;
	fixTime = frameTime;
}

Time::Time()
	:time(0),
	fps(0),
	frameCount(0),
	passedTime(0.f)
{
}


Time::~Time()
{
}

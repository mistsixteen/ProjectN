#include "stdafx.h"
#include "TimeManager.h"

TimeManager::TimeManager(void)
	:time(0),
	fps(0.f),
	frameCount(0),
	passedTime(0.f)
{
}


TimeManager::~TimeManager(void)
{
}

void TimeManager::InitTimeMgr()
{
	QueryPerformanceCounter(&frameTime);
	QueryPerformanceCounter(&fixTime);
	QueryPerformanceCounter(&lastTime);				//���� �ð�
	QueryPerformanceFrequency(&cpuTick);			//�ʴ� CPU ����
}

void TimeManager::SetTime()
{
	QueryPerformanceCounter(&frameTime);

	//1�ʰ� �����ٴ� �ǹ�
	if (frameTime.QuadPart - lastTime.QuadPart > cpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&cpuTick);
		lastTime.QuadPart = frameTime.QuadPart;
	}

	//FPS ���ϱ�
	time = float(frameTime.QuadPart - fixTime.QuadPart) / cpuTick.QuadPart;
	passedTime += time;

	++frameCount;
	if (passedTime > 1.f)
	{
		fps = (float)frameCount / passedTime;
		passedTime = 0.f;
		frameCount = 0;
	}

	//time : 1������ �� ����� �ð�
	time = float(frameTime.QuadPart - fixTime.QuadPart) / cpuTick.QuadPart;
	fixTime = frameTime;

}
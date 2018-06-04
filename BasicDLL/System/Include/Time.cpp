#include "Time.h"



void Time::InitTime()
{
	QueryPerformanceCounter(&frameTime);
	QueryPerformanceCounter(&fixTime);
	QueryPerformanceCounter(&lastTime);				//���� �ð�
	QueryPerformanceFrequency(&cpuTick);			//�ʴ� CPU ����
}

void Time::SetTime()
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
		fps = DWORD(frameCount / passedTime);
		passedTime = 0.f;
		frameCount = 0;
	}

	//time : 1������ �� ����� �ð�
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

#pragma once
/**
* UnHandled exception으로 종료된 프로그램의 정보를 담고 있는 파일을 의미
* 프로그램이 종료될 당시의 메모리 정보와 스택 정보들이 남아있음
* 이 정보를 이용해 프로그램이 종료된 원인을 분석하고 쉽게 UnHandled exception을 수정할 수 있게 됨
*/

class MiniDump
{
public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);
};


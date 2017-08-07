#pragma once

#include<d3dx9.h>
#include<d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#include "Macro.h"
#include "Constant.h"
#include "Struct.h"

#include <iostream>
#include <crtdbg.h>

#include <vector>
#include <map>
#include <list>
#include <algorithm>

// 메모리 누수 전처리 문장
#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
#endif

using namespace std;

extern HWND						g_hWnd;
extern HINSTANCE				hInst;

//장치 대표 전역 변수
extern LPDIRECT3DDEVICE9		device;
extern LPD3DXSPRITE				sprite;

//폰트 출력 전역 변수
extern LPD3DXFONT				font;
extern LPD3DXLINE				line;
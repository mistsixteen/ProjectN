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

#ifdef _DEBUG
// �޸� ���� ��ó�� ����
#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
#endif

using namespace std;

extern HWND						g_hWnd;
extern HINSTANCE				hInst;
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

// �޸� ���� ��ó�� ����
#ifndef _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__,__LINE)
#endif

using namespace std;

extern HWND						g_hWnd;
extern HINSTANCE				hInst;

// ���Ŀ� ī�޶� ��ġ ����
extern D3DXVECTOR3 gCameraEye;
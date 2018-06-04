#pragma once
#pragma once
#ifndef _SYSTEMDEFINE_H
#define _SYSTEMDEFINE_H

#include<d3dx9.h>
#include<d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#include "SystemMacro.h"
#include "SystemConst.h"
#ifdef __cplusplus
#define EXTERN  extern "C"
#else
#define EXTERN
#endif

#ifdef SYSTEM_EXPORT
#define SYSTEM_DLL _declspec(dllexport)
#else
#define SYSTEM_DLL _declspec(dllimport)
#endif

#endif
#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/Registry.h"
#include "../LowLib/Log.h"

#define MAX_STRING_LENGTH		409600

#pragma comment(linker, "/export:PP_Open=_PP_Open@4")
#pragma comment(linker, "/export:PP_Close=_PP_Close@0")
#pragma comment(linker, "/export:PP_Collect=_PP_Collect@16")

typedef struct _PERFORMANCE_COUNTER
{
	DWORD					Index;
	TCHAR					Name[32];
	DWORD					Value;
	PERF_COUNTER_DEFINITION	Data;
} PERFORMANCE_COUNTER;
#include "stdafx.h"

#include "../LowLib/CriticalSection.h"
#include "../LowLib/Registry.h"
#include "../LowLib/Log.h"

#define MAX_STRING_LENGTH		409600

typedef struct _PERFORMANCE_COUNTER
{
	DWORD					Index;
	TCHAR					Name[32];
	DWORD					Value;
	PERF_COUNTER_DEFINITION	Data;
} PERFORMANCE_COUNTER;

// 카운터 값을 저장하는 벡터
static vector<PERFORMANCE_COUNTER>	gCounterVector;

// 동기화 개체
static  CriticalSection				gSync;

// 성능 모니터 헤더. 최대 길이와 카운터 개수들이 들어감
static PERF_OBJECT_TYPE				gHeader;

// 레지스트리에서 읽은 카운터 문자열을 저장
static TCHAR						gReadString[MAX_STRING_LENGTH] = { 0, };

// 카운터 문자열 길이 -> 우선 400k로 정정
static DWORD						gReadStringLength = sizeof(gReadString);

// 레지스트리에서 읽은 카운터 도움말을 저장
static TCHAR						gReadHelpString[MAX_STRING_LENGTH] = { 0, };

// 카운터 도움말 길이 -> 우선 400k로 정정
static DWORD						gReadHelpStringLength = sizeof(gReadHelpString);

// 레지스트리에 있는 Object List 값을 저장한 변수
static TCHAR						gObjectListString[MAX_STRING_LENGTH] = { 0, };

// 마지막 카운터 값
static DWORD						gLastIndex = 0;

// 문자열 마지막
static BYTE							gEnd[2] = { 0, 0 };

// Perflib의 문자열에 추가하거나 값을 가져올 때 사용하는 함수
// 대부분 포인터 연산
BOOL AddString(LPTSTR source, LPTSTR addItem, DWORD &length)
{
	if (!source || !addItem)
		return FALSE;

	CopyMemory(source + length / sizeof(TCHAR) - 1, addItem, _tcslen(addItem) * sizeof(TCHAR));
	length += (DWORD)_tcslen(addItem) * sizeof(TCHAR);

	CopyMemory(source + length / sizeof(TCHAR), gEnd, sizeof(gEnd));
	length += sizeof(gEnd);

	return TRUE;
}

BOOL AddDWORD(LPTSTR source, DWORD addItem, DWORD &length)
{
	if (!source)
		return FALSE;

	TCHAR AddItem[16] = { 0, };
	_sntprintf(AddItem, 16, _T("%d"), addItem);

	CopyMemory(source + length / sizeof(TCHAR) - 1, AddItem, _tcslen(AddItem) * sizeof(TCHAR));
	length += (DWORD)_tcslen(AddItem) * sizeof(TCHAR);

	CopyMemory(source + length / sizeof(TCHAR), gEnd, sizeof(gEnd));
	length += sizeof(gEnd);

	return TRUE;
}

BOOL GetIndex(LPTSTR source, LPTSTR counterName, DWORD &index)
{
	TCHAR *PreviosString = NULL;
	DWORD Length = (DWORD)_tcslen(source) + 1;

	// 처음일리가 없다
	if (!_tcscmp(source, counterName))
		return FALSE;

	while (Length)
	{
		if (!_tcscmp(source + Length, counterName))
		{
			index = _ttoi(PreviosString);

			return TRUE;
		}

		PreviosString = source + Length;
		Length += (DWORD)_tcslen(source + Length) + 1;
	}

	return FALSE;
}

BOOL GetName(LPTSTR source, DWORD index, LPTSTR counterName)
{
	TCHAR   *PreviosString = NULL;
	DWORD   Length = (DWORD)_tcslen(source) + 1;
	TCHAR   Index[16] = { 0, };

	_itot(index, Index, 10);

	// 처음일리가 없다
	if (!_tcscmp(source, Index))
		return FALSE;

	while (Length)
	{
		if (!_tcscmp(source + Length, Index))
		{
			Length += (DWORD)_tcslen(source + Length) + 1;

			_tcscpy(counterName, source + Length);

			return TRUE;
		}

		PreviosString = source + Length;
		Length += (DWORD)_tcslen(source + Length) + 1;
	}

	return FALSE;
}

extern "C"
{
	// 서버 시작 시 실행하는 함수
	BOOL __declspec(dllexport) Begin(LPTSTR dllPath)
	{
		gSync.Enter();		// 동기화 개체

							// dllPath 값이 없을 경우 실행하지 않음
		if (!dllPath) {
			gSync.Leave();
			return FALSE;
		}

		// Collect에서 사용할 헤더를 초기화
		ZeroMemory(&gHeader, sizeof(gHeader));

		Registry registry;

		// 레지스트리에서 카운터 값과 도움말 값을 가져옴
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.GetValue(L"Counter", gReadString, &gReadStringLength);
		registry.GetValue(L"Help", gReadHelpString, &gReadHelpStringLength);
		registry.Close();

		// 레지스트리에서 마지막 카운터 값을 가져옴
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		// Performance 레지스트리가 없을 경우 생성
		if (!registry.Open(HKEY_LOCAL_MACHINE,
			L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")) {
			registry.CreateKey(HKEY_LOCAL_MACHINE,
				L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance");

			// 카운터와 도움말 번호를 입력
			_sntprintf(gObjectListString, MAX_STRING_LENGTH, L"%d", gLastIndex + 2);
			registry.SetValue(L"Object List", gObjectListString);

			// Open, Close, Collect를 호출
			registry.SetValue(L"Open", L"PP_Open");
			registry.SetValue(L"Close", L"PP_Close");
			registry.SetValue(L"Collect", L"PP_Collect");
			// DLL 위치를 등록
			registry.SetValue(L"Library", dllPath);

			registry.Close();

			// Perflib의 Last 값 업데이트
			registry.Open(HKEY_LOCAL_MACHINE,
				L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
			registry.SetValue(L"Last Counter", gLastIndex + 2);
			registry.SetValue(L"Last Help", gLastIndex + 3);
			registry.Close();

			// gReadString을 업데이트 한 후에 레지스트리에 써 줌
			AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
			AddString(gReadString, L"Performance Provider", gReadStringLength);

			AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
			AddString(gReadHelpString, L"Performance Provider Help", gReadHelpStringLength);

			// MultiSz형의 문자열 데이터를 써줌
			registry.Open(HKEY_LOCAL_MACHINE,
				L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
			registry.SetValueForMultiSize(L"Counter", gReadString, gReadStringLength);
			registry.SetValueForMultiSize(L"Help", gReadHelpString, gReadHelpStringLength);
			registry.Close();

			gLastIndex += 2;
		}
		// 이미 레지스트리가 존재하면 값을 가져옴
		else {
			DWORD MaxStringLength = MAX_STRING_LENGTH;
			registry.GetValue(L"Object List", gObjectListString, &MaxStringLength);
			registry.Close();
		}

		// 마지막 카운터와 처음 카운터를 알아서 루프를 돌면서 그 정보를 Vector에 입력
		TCHAR Seperator[] = L"";
		LPTSTR Token = NULL;
		Token = _tcstok(gObjectListString, Seperator);
		DWORD FirstIndex = _ttoi(Token);

		// Collect에서 사용할 헤더 값을 초기화
		gHeader.TotalByteLength = (DWORD)gCounterVector.size() *
			sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader) +
			sizeof(DWORD) *
			gCounterVector.size() +
			sizeof(DWORD);
		gHeader.DefinitionLength = (DWORD)gCounterVector.size() *
			sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader);
		gHeader.HeaderLength = sizeof(gHeader);
		gHeader.ObjectNameTitleIndex = FirstIndex;
		gHeader.ObjectNameTitle = NULL;
		gHeader.ObjectHelpTitleIndex = FirstIndex + 1;
		gHeader.ObjectHelpTitleIndex = NULL;
		gHeader.DetailLevel = PERF_DETAIL_NOVICE;
		gHeader.NumCounters = (DWORD)gCounterVector.size();
		gHeader.DefaultCounter = 0;
		gHeader.NumInstances = PERF_NO_INSTANCES;
		gHeader.CodePage = 0;

		ZeroMemory(&gHeader.PerfTime, sizeof(gHeader.PerfTime));
		ZeroMemory(&gHeader.PerfFreq, sizeof(gHeader.PerfFreq));

		// 처음은 무시
		Token = _tcstok(NULL, Seperator);

		// 카운터 값을 입력해줌
		while (Token) {
			DWORD Index = _ttoi(Token);

			PERFORMANCE_COUNTER pc;
			ZeroMemory(&pc, sizeof(pc));

			pc.Index = Index;
			pc.Value = 0;

			pc.Data.ByteLength = sizeof(pc.Data);
			pc.Data.CounterNameTitleIndex = Index;
			pc.Data.CounterNameTitle = NULL;
			pc.Data.CounterHelpTitleIndex = Index + 1;
			pc.Data.CounterHelpTitle = NULL;
			pc.Data.DefaultScale = 0;
			pc.Data.DetailLevel = PERF_DETAIL_NOVICE;
			pc.Data.CounterType = PERF_COUNTER_RAWCOUNT;
			pc.Data.CounterSize = sizeof(DWORD);
			pc.Data.CounterOffset = sizeof(DWORD);

			GetName(gReadString, Index, pc.Name);

			gCounterVector.push_back(pc);

			Token = _tcstok(NULL, Seperator);
		}

		gHeader.TotalByteLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader) +
			sizeof(DWORD) * gCounterVector.size() +
			sizeof(DWORD);
		gHeader.DefinitionLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader);
		gHeader.NumCounters = (DWORD)gCounterVector.size();

		gSync.Leave();

		return TRUE;
	}

	BOOL __declspec(dllexport) AddCounter(LPTSTR name)
	{
		gSync.Enter();			// 동기화 개체

								// 새로운 카운터의 이름이 정해지지 않으면 끝냄
		if (!name) {
			gSync.Leave();
			return FALSE;
		}

		// 기존의 카운터 중에 같은 이름이 있는지 확인
		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			if (!_tcscmp(gCounterVector[i].Name, name)) {
				gSync.Leave();
				return FALSE;
			}
		}

		// Last Index 한번 확인
		Registry registry;
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		// 1. 스트링 쪽에 넣어주고
		AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
		AddString(gReadString, name, gReadStringLength);

		// 도움말도 입력
		AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
		AddString(gReadHelpString, name, gReadHelpStringLength);

		_sntprintf(gObjectListString, MAX_STRING_LENGTH, L"%s %d", gObjectListString, gLastIndex + 2);

		// 값을 업데이트
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.SetValueForMultiSize(L"Counter", gReadString, gReadStringLength);
		registry.SetValueForMultiSize(L"Help", gReadHelpString, gReadHelpStringLength);
		registry.Close();

		// 2. Perflib쪽 LastIndex Update
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");

		registry.SetValue(L"Last Counter", gLastIndex + 2);
		registry.SetValue(L"Last Help", gLastIndex + 3);
		registry.Close();

		// 3. Perfomance Provider 쪽 추가
		registry.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance");
		registry.SetValue(L"Object List", gObjectListString);
		registry.Close();

		// 4. Vector 추가 (메모리에 현재 로드되어 있는 데이터 업데이트)
		PERFORMANCE_COUNTER pc;

		pc.Index = gLastIndex + 2;
		pc.Value = 0;

		pc.Data.ByteLength = sizeof(pc.Data);
		pc.Data.CounterNameTitleIndex = gLastIndex + 2;
		pc.Data.CounterNameTitle = NULL;
		pc.Data.CounterHelpTitleIndex = gLastIndex + 3;
		pc.Data.CounterHelpTitle = NULL;
		pc.Data.DefaultScale = 0;
		pc.Data.DetailLevel = PERF_DETAIL_NOVICE;
		pc.Data.CounterType = PERF_COUNTER_RAWCOUNT;
		pc.Data.CounterSize = sizeof(DWORD);
		pc.Data.CounterOffset = sizeof(DWORD);

		GetName(gReadString, gLastIndex + 2, pc.Name);

		gCounterVector.push_back(pc);

		// 5. Header 변경
		gHeader.TotalByteLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader) +
			sizeof(DWORD) * gCounterVector.size() +
			sizeof(DWORD);
		gHeader.DefinitionLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
			sizeof(gHeader);
		gHeader.NumCounters = (DWORD)gCounterVector.size();

		gLastIndex = gLastIndex + 2;

		gSync.Leave();

		return TRUE;
	}

	// 서버에서 카운터 값이 변경되었을 때 업데이트하는 함수
	BOOL __declspec(dllexport) UpdateCounter(LPTSTR name, DWORD value)
	{
		gSync.Enter();			// 동기화 개체

								// 벡터에서 해당 이름의 카운터를 찾아 값을 업데이트
		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			if (!_tcscmp(gCounterVector[i].Name, name)) {
				gCounterVector[i].Value = value;
				gSync.Leave();
				return FALSE;
			}
		}

		gSync.Leave();

		return TRUE;
	}

	// 여기까지가 서버 쪽에서 사용하는 함수
	//
	// 서보 쪽에서 자신이 필요한 카운터를 레지스트리에 추가하고 그 값을 읽어
	// 메모리에 가지고 있다가 값이 변경되면 업데이트

	// 성능 모니터에서 DLL이 처음 로드 되었을 때 호출되는 함수
	DWORD __declspec(dllexport) WINAPI PP_Open(LPWSTR lpDevNames)
	{
		gSync.Enter();

		// 우선 헤더 값을 초기화하고
		ZeroMemory(&gHeader, sizeof(gHeader));

		Registry registry;

		// 레지스트리에서 카운터 값과 도움말 값을 가져와서 저장
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.GetValue(L"Counter", gReadString, &gReadStringLength);
		registry.GetValue(L"Help", gReadHelpString, &gReadHelpStringLength);
		registry.Close();

		// 마지막 번호 값도 가져와서 저장
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		if (registry.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")) {
			DWORD MaxStringLength = MAX_STRING_LENGTH;

			// 카운터 번호를 받아와서
			registry.GetValue(L"Object List", gObjectListString, &MaxStringLength);

			registry.Close();

			TCHAR Seperator[] = L"";
			LPTSTR Token = NULL;
			Token = _tcstok(gObjectListString, Seperator);

			DWORD FirstIndex = _ttoi(Token);

			// 헤더를 구성
			gHeader.TotalByteLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
				sizeof(gHeader) +
				sizeof(DWORD) * gCounterVector.size() +
				sizeof(DWORD);
			gHeader.DefinitionLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
				sizeof(gHeader);
			gHeader.HeaderLength = sizeof(gHeader);
			gHeader.ObjectNameTitleIndex = FirstIndex;
			gHeader.ObjectNameTitle = NULL;
			gHeader.ObjectHelpTitleIndex = FirstIndex + 1;
			gHeader.ObjectHelpTitle = NULL;
			gHeader.DetailLevel = PERF_DETAIL_NOVICE;
			gHeader.NumCounters = (DWORD)gCounterVector.size();
			gHeader.DefaultCounter = 0;
			gHeader.NumInstances = PERF_NO_INSTANCES;
			gHeader.CodePage = 0;

			ZeroMemory(&gHeader.PerfTime, sizeof(gHeader.PerfTime));
			ZeroMemory(&gHeader.PerfFreq, sizeof(gHeader.PerfFreq));

			// 처음은 무시
			Token = _tcstok(NULL, Seperator);

			// 반복하면서 카운터 값을 구성해서 벡터에 넣음
			while (Token) {
				DWORD Index = _ttoll(Token);

				PERFORMANCE_COUNTER pc;
				ZeroMemory(&pc, sizeof(pc));

				pc.Index = Index;
				pc.Value = 0;

				pc.Data.ByteLength = sizeof(pc.Data);
				pc.Data.CounterNameTitleIndex = gLastIndex;
				pc.Data.CounterNameTitle = NULL;
				pc.Data.CounterHelpTitleIndex = gLastIndex + 1;
				pc.Data.CounterHelpTitle = NULL;
				pc.Data.DefaultScale = 0;
				pc.Data.DetailLevel = PERF_DETAIL_NOVICE;
				pc.Data.CounterType = PERF_COUNTER_RAWCOUNT;
				pc.Data.CounterSize = sizeof(DWORD);
				pc.Data.CounterOffset = sizeof(DWORD);

				GetName(gReadString, Index, pc.Name);

				gCounterVector.push_back(pc);

				Token = _tcstok(NULL, Seperator);
			}

			gHeader.TotalByteLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
				sizeof(gHeader) +
				sizeof(DWORD) * gCounterVector.size() +
				sizeof(DWORD);
			gHeader.DefinitionLength = (DWORD)gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) +
				sizeof(gHeader);
			gHeader.NumCounters = (DWORD)gCounterVector.size();
		}

		gSync.Leave();

		return (ERROR_SUCCESS);
	}

	// 종료 처리 함수. 특별한 작업은 없음
	DWORD __declspec(dllexport) WINAPI PP_Close(VOID)
	{
		return (ERROR_SUCCESS);
	}

	// 현재의 카운터 값을 성능 모니터로 보내는 함수
	DWORD __declspec(dllexport) WINAPI PP_Collect(LPWSTR lpValueName,
		LPVOID* lppData,
		LPDWORD lpcbTotalBytes,
		LPDWORD lpNumObjectTypes)
	{
		gSync.Enter();
		*lpcbTotalBytes = gHeader.TotalByteLength;
		*lpNumObjectTypes = (DWORD)gCounterVector.size() + 1;

		PBYTE* Data = (PBYTE*)lppData;

		// 설명했던 메모리 구성대로 복사해서 데이터를 만듦
		CopyMemory(*Data, &gHeader, sizeof(gHeader));
		*Data += sizeof(gHeader);

		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			CopyMemory(*Data, &gCounterVector[i].Data, sizeof(gCounterVector[i].Data));
			*Data += sizeof(gCounterVector[i].Data);
		}

		DWORD ValueLength = (DWORD)(sizeof(DWORD) * gCounterVector.size());
		CopyMemory(*Data, &ValueLength, sizeof(DWORD));
		*Data += sizeof(DWORD);

		// 카운터 값을 추가
		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			CopyMemory(*Data, &gCounterVector[i].Value, sizeof(DWORD));
			*Data += sizeof(DWORD);
		}

		gSync.Leave();

		return (ERROR_SUCCESS);
	}
}

#pragma comment(linker, "/export:PP_Open=_PP_Open@4")
#pragma comment(linker, "/export:PP_Close=_PP_Close@0")
#pragma comment(linker, "/export:PP_Collect=_PP_Collect@16")
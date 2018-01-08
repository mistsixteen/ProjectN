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

// ī���� ���� �����ϴ� ����
static vector<PERFORMANCE_COUNTER>	gCounterVector;

// ����ȭ ��ü
static  CriticalSection				gSync;

// ���� ����� ���. �ִ� ���̿� ī���� �������� ��
static PERF_OBJECT_TYPE				gHeader;

// ������Ʈ������ ���� ī���� ���ڿ��� ����
static TCHAR						gReadString[MAX_STRING_LENGTH] = { 0, };

// ī���� ���ڿ� ���� -> �켱 400k�� ����
static DWORD						gReadStringLength = sizeof(gReadString);

// ������Ʈ������ ���� ī���� ������ ����
static TCHAR						gReadHelpString[MAX_STRING_LENGTH] = { 0, };

// ī���� ���� ���� -> �켱 400k�� ����
static DWORD						gReadHelpStringLength = sizeof(gReadHelpString);

// ������Ʈ���� �ִ� Object List ���� ������ ����
static TCHAR						gObjectListString[MAX_STRING_LENGTH] = { 0, };

// ������ ī���� ��
static DWORD						gLastIndex = 0;

// ���ڿ� ������
static BYTE							gEnd[2] = { 0, 0 };

// Perflib�� ���ڿ��� �߰��ϰų� ���� ������ �� ����ϴ� �Լ�
// ��κ� ������ ����
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

	// ó���ϸ��� ����
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

	// ó���ϸ��� ����
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
	// ���� ���� �� �����ϴ� �Լ�
	BOOL __declspec(dllexport) Begin(LPTSTR dllPath)
	{
		gSync.Enter();		// ����ȭ ��ü

							// dllPath ���� ���� ��� �������� ����
		if (!dllPath) {
			gSync.Leave();
			return FALSE;
		}

		// Collect���� ����� ����� �ʱ�ȭ
		ZeroMemory(&gHeader, sizeof(gHeader));

		Registry registry;

		// ������Ʈ������ ī���� ���� ���� ���� ������
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.GetValue(L"Counter", gReadString, &gReadStringLength);
		registry.GetValue(L"Help", gReadHelpString, &gReadHelpStringLength);
		registry.Close();

		// ������Ʈ������ ������ ī���� ���� ������
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		// Performance ������Ʈ���� ���� ��� ����
		if (!registry.Open(HKEY_LOCAL_MACHINE,
			L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")) {
			registry.CreateKey(HKEY_LOCAL_MACHINE,
				L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance");

			// ī���Ϳ� ���� ��ȣ�� �Է�
			_sntprintf(gObjectListString, MAX_STRING_LENGTH, L"%d", gLastIndex + 2);
			registry.SetValue(L"Object List", gObjectListString);

			// Open, Close, Collect�� ȣ��
			registry.SetValue(L"Open", L"PP_Open");
			registry.SetValue(L"Close", L"PP_Close");
			registry.SetValue(L"Collect", L"PP_Collect");
			// DLL ��ġ�� ���
			registry.SetValue(L"Library", dllPath);

			registry.Close();

			// Perflib�� Last �� ������Ʈ
			registry.Open(HKEY_LOCAL_MACHINE,
				L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
			registry.SetValue(L"Last Counter", gLastIndex + 2);
			registry.SetValue(L"Last Help", gLastIndex + 3);
			registry.Close();

			// gReadString�� ������Ʈ �� �Ŀ� ������Ʈ���� �� ��
			AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
			AddString(gReadString, L"Performance Provider", gReadStringLength);

			AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
			AddString(gReadHelpString, L"Performance Provider Help", gReadHelpStringLength);

			// MultiSz���� ���ڿ� �����͸� ����
			registry.Open(HKEY_LOCAL_MACHINE,
				L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
			registry.SetValueForMultiSize(L"Counter", gReadString, gReadStringLength);
			registry.SetValueForMultiSize(L"Help", gReadHelpString, gReadHelpStringLength);
			registry.Close();

			gLastIndex += 2;
		}
		// �̹� ������Ʈ���� �����ϸ� ���� ������
		else {
			DWORD MaxStringLength = MAX_STRING_LENGTH;
			registry.GetValue(L"Object List", gObjectListString, &MaxStringLength);
			registry.Close();
		}

		// ������ ī���Ϳ� ó�� ī���͸� �˾Ƽ� ������ ���鼭 �� ������ Vector�� �Է�
		TCHAR Seperator[] = L"";
		LPTSTR Token = NULL;
		Token = _tcstok(gObjectListString, Seperator);
		DWORD FirstIndex = _ttoi(Token);

		// Collect���� ����� ��� ���� �ʱ�ȭ
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

		// ó���� ����
		Token = _tcstok(NULL, Seperator);

		// ī���� ���� �Է�����
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
		gSync.Enter();			// ����ȭ ��ü

								// ���ο� ī������ �̸��� �������� ������ ����
		if (!name) {
			gSync.Leave();
			return FALSE;
		}

		// ������ ī���� �߿� ���� �̸��� �ִ��� Ȯ��
		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			if (!_tcscmp(gCounterVector[i].Name, name)) {
				gSync.Leave();
				return FALSE;
			}
		}

		// Last Index �ѹ� Ȯ��
		Registry registry;
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		// 1. ��Ʈ�� �ʿ� �־��ְ�
		AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
		AddString(gReadString, name, gReadStringLength);

		// ���򸻵� �Է�
		AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
		AddString(gReadHelpString, name, gReadHelpStringLength);

		_sntprintf(gObjectListString, MAX_STRING_LENGTH, L"%s %d", gObjectListString, gLastIndex + 2);

		// ���� ������Ʈ
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.SetValueForMultiSize(L"Counter", gReadString, gReadStringLength);
		registry.SetValueForMultiSize(L"Help", gReadHelpString, gReadHelpStringLength);
		registry.Close();

		// 2. Perflib�� LastIndex Update
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");

		registry.SetValue(L"Last Counter", gLastIndex + 2);
		registry.SetValue(L"Last Help", gLastIndex + 3);
		registry.Close();

		// 3. Perfomance Provider �� �߰�
		registry.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance");
		registry.SetValue(L"Object List", gObjectListString);
		registry.Close();

		// 4. Vector �߰� (�޸𸮿� ���� �ε�Ǿ� �ִ� ������ ������Ʈ)
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

		// 5. Header ����
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

	// �������� ī���� ���� ����Ǿ��� �� ������Ʈ�ϴ� �Լ�
	BOOL __declspec(dllexport) UpdateCounter(LPTSTR name, DWORD value)
	{
		gSync.Enter();			// ����ȭ ��ü

								// ���Ϳ��� �ش� �̸��� ī���͸� ã�� ���� ������Ʈ
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

	// ��������� ���� �ʿ��� ����ϴ� �Լ�
	//
	// ���� �ʿ��� �ڽ��� �ʿ��� ī���͸� ������Ʈ���� �߰��ϰ� �� ���� �о�
	// �޸𸮿� ������ �ִٰ� ���� ����Ǹ� ������Ʈ

	// ���� ����Ϳ��� DLL�� ó�� �ε� �Ǿ��� �� ȣ��Ǵ� �Լ�
	DWORD __declspec(dllexport) WINAPI PP_Open(LPWSTR lpDevNames)
	{
		gSync.Enter();

		// �켱 ��� ���� �ʱ�ȭ�ϰ�
		ZeroMemory(&gHeader, sizeof(gHeader));

		Registry registry;

		// ������Ʈ������ ī���� ���� ���� ���� �����ͼ� ����
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib\\012");
		registry.GetValue(L"Counter", gReadString, &gReadStringLength);
		registry.GetValue(L"Help", gReadHelpString, &gReadHelpStringLength);
		registry.Close();

		// ������ ��ȣ ���� �����ͼ� ����
		registry.Open(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\WindowsNT\\CurrentVersion\\Perflib");
		registry.GetValue(L"Last Counter", &gLastIndex);
		registry.Close();

		if (registry.Open(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")) {
			DWORD MaxStringLength = MAX_STRING_LENGTH;

			// ī���� ��ȣ�� �޾ƿͼ�
			registry.GetValue(L"Object List", gObjectListString, &MaxStringLength);

			registry.Close();

			TCHAR Seperator[] = L"";
			LPTSTR Token = NULL;
			Token = _tcstok(gObjectListString, Seperator);

			DWORD FirstIndex = _ttoi(Token);

			// ����� ����
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

			// ó���� ����
			Token = _tcstok(NULL, Seperator);

			// �ݺ��ϸ鼭 ī���� ���� �����ؼ� ���Ϳ� ����
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

	// ���� ó�� �Լ�. Ư���� �۾��� ����
	DWORD __declspec(dllexport) WINAPI PP_Close(VOID)
	{
		return (ERROR_SUCCESS);
	}

	// ������ ī���� ���� ���� ����ͷ� ������ �Լ�
	DWORD __declspec(dllexport) WINAPI PP_Collect(LPWSTR lpValueName,
		LPVOID* lppData,
		LPDWORD lpcbTotalBytes,
		LPDWORD lpNumObjectTypes)
	{
		gSync.Enter();
		*lpcbTotalBytes = gHeader.TotalByteLength;
		*lpNumObjectTypes = (DWORD)gCounterVector.size() + 1;

		PBYTE* Data = (PBYTE*)lppData;

		// �����ߴ� �޸� ������� �����ؼ� �����͸� ����
		CopyMemory(*Data, &gHeader, sizeof(gHeader));
		*Data += sizeof(gHeader);

		for (DWORD i = 0; i < gCounterVector.size(); ++i) {
			CopyMemory(*Data, &gCounterVector[i].Data, sizeof(gCounterVector[i].Data));
			*Data += sizeof(gCounterVector[i].Data);
		}

		DWORD ValueLength = (DWORD)(sizeof(DWORD) * gCounterVector.size());
		CopyMemory(*Data, &ValueLength, sizeof(DWORD));
		*Data += sizeof(DWORD);

		// ī���� ���� �߰�
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
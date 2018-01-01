// PacketGenerator.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

// 한 라인의 최대 길이
#define MAX_LINE_LENGTH			255
// 한 프로토콜 당 최대 가질 수 있는 파라미터 개수
#define MAX_PARAMETER_COUNT		255
// 최대 프로토콜 개수
#define MAX_PROTOCOL_COUNT		255

// 프로토콜은 크게 프로토콜과 파라미터로 나눠짐
// 파라미터
typedef struct _PARAMETER
{
	// 파라미터의 형
	TCHAR Type[MAX_LINE_LENGTH];
	// 파라미터 변수 명
	TCHAR Name[MAX_LINE_LENGTH];
	// 파라미터 길이
	DWORD Length;		// _BYTE형에서만 길이가 있음
}PARAMETER;

// 프로토콜
typedef struct _PROTOCOL
{
	// 프로토콜 이름
	TCHAR Name[MAX_LINE_LENGTH];
	// 파라미터 개수
	DWORD ParametersCount;
	// 파라미터 정보 : 배열로 잡고 최대 파라미터를 가질 수 있는 개수는 255개
	PARAMETER Parameters[MAX_PARAMETER_COUNT];
}PROTOCOL;

int _tmain(int argc, _TCHAR* argv[])
{
	// 파일 이름 파라미터가 없다면 미실행
	if (argc == 1)
		return 0;

	// 프로토콜 정보를 저장할 PROTOCOL 구조체 배열을 생성
	PROTOCOL* Protocol = new PROTOCOL[MAX_PROTOCOL_COUNT];
	// 현재 마지막으로 저장된 프로토콜 구조체의 위치
	DWORD ProtocolPos = 0;

	// 저장할 structure 파일 이름
	TCHAR StructureFileName[MAX_PATH] = { 0, };
	// 저장할 Read 함수 파일 이름
	TCHAR ReadPacketFileName[MAX_PATH] = { 0, };
	// 저장할 Write 함수 파일 이름
	TCHAR WritePacketFileName[MAX_PATH] = { 0, };

	// structure 파일은 뒤에 _Structure.h를 붙임
	// read 파일은 뒤에 _ReadPacket.h를 붙임
	// write 파일은 뒤에 _WritePacket.h를 붙임
	_sntprintf(StructureFileName, MAX_PATH, L"%s_Structure.h", argv[2]);
	_sntprintf(ReadPacketFileName, MAX_PATH, L"%s_ReadPacket.h", argv[2]);
	_sntprintf(WritePacketFileName, MAX_PATH, L"%s_WritePacket.h", argv[2]);

	// 프로토콜 정보를 저장할 메모리를 초기화
	ZeroMemory(Protocol, sizeof(PROTOCOL) * MAX_PROTOCOL_COUNT);
	
	// 원본 파일 여기서는 Protocol.h를 엶
	FILE* SourceFile = _tfopen(argv[1], L"r");

	// 한 라인을 읽어서 저장할 버퍼
	TCHAR Data[MAX_LINE_LENGTH] = { 0, };
	// 현재 파라미터를 읽는 중인지를 확인하는 FLAG 변수
	// 파라미터는 항상 /* */ 안에 들어있기 때문에 체크할 수 있음
	BOOL IsParameterOpen = FALSE;
	// 현재 몇 번째 파라미터인지를 확인하는 변수
	DWORD ParameterPos = 0;

	// 한 라인별로 읽어서 프로토콜을 메모리에 저장
	while (_fgetts(Data, MAX_LINE_LENGTH, SourceFile)) {
		// 공백, 탭, 엔터 등을 정리한 데이터를 저장하는 버퍼
		TCHAR TempData[MAX_LINE_LENGTH] = { 0, };
		// 현재 읽고 있는 버퍼의 위치
		DWORD TempDataPos = 0;
		for (DWORD i = 0; i < _tcslen(Data); ++i) {
			// 읽은 한 라인 내의 공백, 탭, 엔터 값을 없애줌
			if (Data[i] != ' ' && Data[i] != '\t' && Data[i] != '\n' && Data[i] != '\r') {
				// TempData 버퍼에 정리해서 넣어줌
				TempData[TempDataPos++] = Data[i];
			}
		}

		// 이 라인의 데이터가 프로토콜 이름인지 파라미터인지 구분
		// 파라미터일 경우 형과 변수 명으로 이뤄지고 프로토콜 이름에 따라감
		// 파라미터는 최대 255개까지 줄 수 있음
		
		// 라인 안에 ','가 있으면 프로토콜
		// enum의 특징상 ','로 확인
		// 이부분 때문에 파라미터에는 ','가 들어가면 안됨
		TCHAR* Pos = _tcsstr(TempData, L",");
		if (Pos) {
			// 프로토콜일 경우 VERSION이라는 문자열을 포함한 경우 무시
			// 포함하고 있지 않은 경우, 프로토콜 위치를 하나 증가시켜서 프로토콜 이름을 복사
			if (!_tcsstr(TempData, L"VERSION")) {
				_tcsncpy(Protocol[ProtocolPos++].Name, TempData, Pos - TempData);
			}
			else {

			}
		}
		// 프로토콜 이름이 아닐 경우
		// 이 경우엔 쓸모없는 데이터이거나 파라미터 정보일 것
		else {
			// 데이터에 파라미터 시작 기호가 있을 경우
			if (_tcsstr(TempData, L"/*")) {
				// 파라미터 오픈 FLAG를 TRUE로 만듦
				IsParameterOpen = TRUE;
				// 파라미터 순서 변수를 초기화
				ParameterPos = 0;
				// 다음 라인을 읽음
				continue;
			}

			// 데이터에 파라미터 종료 기호가 있을 경우
			if (_tcsstr(TempData, L"*/")) {
				// 파라미터 오픈 FLAG를 FALSE
				IsParameterOpen = FALSE;
				// 해당 프로토콜의 파라미터 개수를 입력
				Protocol[ProtocolPos - 1].ParametersCount = ParameterPos;
				// 파라미터 순서를 초기화
				ParameterPos = 0;
				// 다음 라인을 읽음
				continue;
			}

			// 파라미터 열기 기호나 닫기 기호가 아닐 경우
			if (_tcslen(TempData) != 0) {
				// 파라미터가 열린 상태일 경우 (이 경우는 파라미터 데이터를 읽어오는 것)
				if (IsParameterOpen) {
					// 무언가 있을 경우
					// 파라미터인데, 형, 이름, 사이즈를 분리
					Pos = _tcsstr(TempData, L":");

					// 라인에 형, 변수명 구분자가 있을 경우
					if (Pos) {
						// 파라미터 형 복사
						_tcsncpy(Protocol[ProtocolPos - 1].Parameters[ParameterPos++].Type, 
									TempData, Pos - TempData);
					}
					// : 기호가 없을 경우 다음 라인을 읽음
					else {
						continue;
					}
					// :가 있는 라인에 [ 기호가 있을 경우 그 위치를 찾음
					TCHAR *StartPos = _tcsstr(TempData, L"[");
					// 그 위치가 0이 아닐 경우
					if (StartPos) {
						// ] 기호가 있는 위치를 찾음
						TCHAR* EndPos = _tcsstr(TempData, L"]");
						TCHAR Length[MAX_LINE_LENGTH] = { 0, };

						// [시작부터 ]끝까지의 길이를 구해서 선언한 파라미터가 배열일 경우
						// 그 배열 크기를 구해서 Length에 저장
						_tcsncpy(Length, StartPos + 1, EndPos - StartPos - 1);
						// 프로토콜 구조체에 저장함
						Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Length = _tstoi(Length);

						// 파라미터 이름을 복사
						_tcsncpy(Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Name,
								Pos + 1, StartPos - Pos - 1);
					}
					else {
						// 만약 [ 기호가 없을 경우는 배열형이 아니기 때문에 길이를 0으로 세팅
						Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Length = 0;
					}
				}
			}
		}
	}

	fclose(SourceFile);

	// 위 과정을 거치게 되면 Protocol.h 파일을 읽어서
	// 그 내용을 프로토콜 배열에 파라미터 값을 포함해서 저장
	// 아래 내용부터는 저장된 프로토콜 배열에서 Output 파일들을 생성해 내는 과정을 보여줌

	// structure 만들기
	FILE* StructureFile = _tfopen(StructureFileName, L"w");

	_ftprintf(StructureFile, L"pragma once\n\n");

	// 읽은 프로토콜 전체 개수만큼 반복됨
	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// 읽은 프로토콜과 파라미터 정보를 이용해서 구조체를 선언
		_ftprintf(StructureFile, L"typedef struct _S_%s\n{\n", Protocol[i].Name);
		// 읽은 프로토콜의 파라미터 개수만큼 반복
		for (DWORD j = 0; j < Protocol[i].Parameters[j].Length; ++j) {
			// 배열형이 아닌 파라미터일 경우
			if (Protocol[i].Parameters[j].Length == 0) {
				// 데이터 형과 변수명을 써 줌
				_ftprintf(StructureFile, L"\t%s %s;\n",
				Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name);
			}
			// 배열형인 파라미터일 경우
			else {
				// 데이텨형, 변수명, 배열 크기를 써줌
				_ftprintf(StructureFile, L"\t%s %s[%d];",
					Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name,
					Protocol[i].Parameters[j].Length);
			}
		}
		// 마지막으로 구조체를 닫고 이름을 정함
		_ftprintf(StructureFile, L"} S_%s;\n\n", Protocol[i].Name);
	}

	fclose(StructureFile);
	// 위의 작업을 반복하게 되면 모든 프로토콜들의 파라미터를 포함한 구조체가 생성

	// Read 함수 만들기
	FILE* ReadPacketFile = _tfopen(ReadPacketFileName, L"w");

	_ftprintf(ReadPacketFile, L"#pragma once\n\n");

	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// READ 함수 선언, inline 형태로 선언
		// 이 READ 함수들은 위에서 이리 만들어 놓은 구조를 이용해서 데이터를 읽게 됨
		_ftprintf(ReadPacketFile, L"inline VOID READ_%s(BYTE* buffer, S%s &parameter", 
			Protocol[i].Name, Protocol[i].Name);
		// 바이너리에서 데이터를 읽기 위해서 StreamSP 클래스를 사용
		_ftprintf(ReadPacketFile, L"\tStreamSP Stream;\n");
		_ftprintf(ReadPacketFile, L"\tStream->SetBuffer(buffer);\n\n");

		// 해당 프로토콜의 파라미터 개수 반복
		for (DWORD j = 0; j < Protocol[i].ParametersCount; ++j) {
			// 파라미터 형이 INT 형
			if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT")) {
				// ReadInt32 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadInt32(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 DWORD 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD")) {
				// ReadDWORD 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadDWORD(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 DWORD_PTR 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD_PTR")) {
				// ReadDWORD_PTR 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadDWORD_PTR(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 BYTE 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BYTE")) {
				// 만약 파라미터의 길이가 0이면
				if (Protocol[i].Parameters[j].Length == 0) {
					// ReadByte 함수를 통해 1바이트를 구조체에 입력
					_ftprintf(ReadPacketFile, L"\tStream->ReadByte(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// 파라미터의 길이가 0보다 클 경우, 즉 배열인 경우
				else {
					// ReadBytes 함수를 이용해서 버퍼를 복사
					_ftprintf(ReadPacketFile, L"\tStream->ReadBytes(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// 파라미터 형이 WCHAR 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"WCHAR")) {
				// 만약 파라미터의 길이가 0이면
				if (Protocol[i].Parameters[j].Length == 0) {
					// ReadWCHAR 함수를 통해 1바이트를 구조체에 입력
					_ftprintf(ReadPacketFile, L"\tStream->ReadWCHAR(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// 파라미터의 길이가 0보다 클 경우, 즉 배열인 경우
				else {
					// ReadWCHARs 함수를 이용해서 버퍼를 복사
					_ftprintf(ReadPacketFile, L"\tStream->ReadWCHARs(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// 파라미터 형이 FLOAT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"FLOAT")) {
				// ReadFloat 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadFloat(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 INT64 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT64")) {
				// ReadInt64 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadInt64(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 USHORT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"USHORT")) {
				// ReadUSHORT 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadUSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 SHORT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"SHORT")) {
				// ReadSHORT 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 BOOL 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BOOL")) {
				// ReadBOOL 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->ReadBOOL(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 그 외의 경우는 BYTE을 이용해서 구조체에 입력
			else {
				_ftprintf(ReadPacketFile, L"\tStream->ReadBytes((BYTE*)parameter.%s, sizeof(%s) * %d);\n",
					Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type, 
					Protocol[i].Parameters[j].Length);
			}
		}
		_ftprintf(ReadPacketFile, L"}\n\n");
	}

	fclose(ReadPacketFile);
	// 위 부분을 종료하면 Read 함수들을 저장해 놓은 파일이 하나 생성

	// 이제보터 살펴볼 Write 함수 파일은 2가지의 Write 함수가 존재
	// 구조체를 이요한 Write 함수와 파라미터를 하나씩 입력하는 파라미터 타입이 존재
	// 구조체 타입의 Write 함수 만들기
	FILE* WritePacketFile = _tfopen(WritePacketFileName, L"w");

	_ftprintf(WritePacketFile, L"#pragma once\n\n");

	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// WRITE 함수 선언, inline 형태로 선언
		// 이 WRITE 함수들은 위에서 이리 만들어 놓은 구조를 이용해서 데이터를 읽게 됨
		_ftprintf(WritePacketFile, L"inline DWORD WRITE_%s(BYTE* buffer, S%s &parameter",
			Protocol[i].Name, Protocol[i].Name);
		// 바이너리에서 데이터를 읽기 위해서 StreamSP 클래스를 사용
		_ftprintf(WritePacketFile, L"\tStreamSP Stream;\n");
		_ftprintf(WritePacketFile, L"\tStream->SetBuffer(buffer);\n\n");

		// 해당 프로토콜의 파라미터 개수 반복
		for (DWORD j = 0; j < Protocol[i].ParametersCount; ++j) {
			// 파라미터 형이 INT 형
			if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT")) {
				// WriteInt32 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteInt32(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 DWORD 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD")) {
				// WriteDWORD 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteDWORD(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 DWORD_PTR 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD_PTR")) {
				// WriteDWORD_PTR 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteDWORD_PTR(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 BYTE 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BYTE")) {
				// 만약 파라미터의 길이가 0이면
				if (Protocol[i].Parameters[j].Length == 0) {
					// WriteByte 함수를 통해 1바이트를 구조체에 입력
					_ftprintf(WritePacketFile, L"\tStream->WriteByte(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// 파라미터의 길이가 0보다 클 경우, 즉 배열인 경우
				else {
					// WriteBytes 함수를 이용해서 버퍼를 복사
					_ftprintf(WritePacketFile, L"\tStream->WriteBytes(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// 파라미터 형이 WCHAR 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"WCHAR")) {
				// 만약 파라미터의 길이가 0이면
				if (Protocol[i].Parameters[j].Length == 0) {
					// WriteWCHAR 함수를 통해 1바이트를 구조체에 입력
					_ftprintf(WritePacketFile, L"\tStream->WriteWCHAR(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// 파라미터의 길이가 0보다 클 경우, 즉 배열인 경우
				else {
					// WriteWCHARs 함수를 이용해서 버퍼를 복사
					_ftprintf(WritePacketFile, L"\tStream->WriteWCHARs(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// 파라미터 형이 FLOAT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"FLOAT")) {
				// WriteFloat 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteFloat(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 INT64 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT64")) {
				// WriteInt64 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->WriteInt64(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 USHORT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"USHORT")) {
				// WriteUSHORT 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteUSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 SHORT 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"SHORT")) {
				// WriteSHORT 함수를 통해 데이터를 구조체에 입력
				_ftprintf(WritePacketFile, L"\tStream->WriteSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 파라미터 형이 BOOL 형
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BOOL")) {
				// WriteBOOL 함수를 통해 데이터를 구조체에 입력
				_ftprintf(ReadPacketFile, L"\tStream->WriteBOOL(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// 그 외의 경우는 BYTE을 이용해서 구조체에 입력
			else {
				_ftprintf(WritePacketFile, L"\tStream->WriteBytes((BYTE*)parameter.%s, sizeof(%s) * %d);\n",
					Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type,
					Protocol[i].Parameters[j].Length);
			}
		}
		_ftprintf(WritePacketFile, L"\n\treturn Stream->GetLength();\n}\n\n");
	}

	// 같은 함수 이름에 파라미터만 ㄷ라리해서 파라미터 타입 함수를 제작
	// Write 함수 파라미터 타입
	// 전체 프로토콜 개수만큼 반복
	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// Write 함수의 원형을 만듦
		// WRITE 함수 선언, inline 형태로 선언
		// 이 WRITE 함수들은 위에서 이리 만들어 놓은 구조를 이용해서 데이터를 읽게 됨
		_ftprintf(WritePacketFile, L"inline DWORD WRITE_%s(BYTE* buffer, S%s &parameter",
			Protocol[i].Name, Protocol[i].Name);

		for (DWORD j = 0; j < Protocol[i].ParametersCount; j++)
		{
			if (Protocol[i].Parameters[j].Length > 0)
				_ftprintf(WritePacketFile, _T(", %s *%s"), Protocol[i].Parameters[j].Type, 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else
				_ftprintf(WritePacketFile, _T(", %s %s"), Protocol[i].Parameters[j].Type, 
					_tcslwr(Protocol[i].Parameters[j].Name));
		}

		_ftprintf(WritePacketFile, _T(")\n{\n"));
		_ftprintf(WritePacketFile, _T("\tStreamSP Stream;\n"));
		_ftprintf(WritePacketFile, _T("\tStream->SetBuffer(buffer);\n\n"));

		for (DWORD j = 0; j<Protocol[i].ParametersCount; j++)
		{
			if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt32(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("DWORD_PTR")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteDWORD_PTR(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BYTE")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteByte(%s);\n"), 
						_tcslwr(Protocol[i].Parameters[j].Name));
				else
					_ftprintf(WritePacketFile, _T("\tStream->WriteBytes(%s, %d);\n"), 
						_tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("WCHAR")))
			{
				if (Protocol[i].Parameters[j].Length == 0)
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHAR(%s);\n"), 
						_tcslwr(Protocol[i].Parameters[j].Name));
				else
				{
					_ftprintf(WritePacketFile, _T("\tWCHAR _%s[%d] = {0,};\n"), 
						_tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
					_ftprintf(WritePacketFile, _T("\t_tcsncpy(_%s, %s, %d);\n"),
						_tcslwr(Protocol[i].Parameters[j].Name), _tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
					_ftprintf(WritePacketFile, _T("\tStream->WriteWCHARs(_%s, %d);\n"), 
						_tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Length);
				}
			}
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("FLOAT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteFloat(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("INT64")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteInt64(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("USHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteUSHORT(%s);\n"),
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("SHORT")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteSHORT(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, _T("BOOL")))
				_ftprintf(WritePacketFile, _T("\tStream->WriteBOOL(%s);\n"), 
					_tcslwr(Protocol[i].Parameters[j].Name));
			else
				_ftprintf(WritePacketFile, _T("\tStream->WriteBytes((BYTE*) %s, sizeof(%s) * %d);\n"),
					_tcslwr(Protocol[i].Parameters[j].Name), Protocol[i].Parameters[j].Type, 
					Protocol[i].Parameters[j].Length);
		}
		_ftprintf(WritePacketFile, _T("\n\treturn Stream->GetLength();\n}\n\n"));
	}

	fclose(WritePacketFile);

	delete[] Protocol;

	return 0;
}
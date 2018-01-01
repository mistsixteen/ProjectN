// PacketGenerator.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

// �� ������ �ִ� ����
#define MAX_LINE_LENGTH			255
// �� �������� �� �ִ� ���� �� �ִ� �Ķ���� ����
#define MAX_PARAMETER_COUNT		255
// �ִ� �������� ����
#define MAX_PROTOCOL_COUNT		255

// ���������� ũ�� �������ݰ� �Ķ���ͷ� ������
// �Ķ����
typedef struct _PARAMETER
{
	// �Ķ������ ��
	TCHAR Type[MAX_LINE_LENGTH];
	// �Ķ���� ���� ��
	TCHAR Name[MAX_LINE_LENGTH];
	// �Ķ���� ����
	DWORD Length;		// _BYTE�������� ���̰� ����
}PARAMETER;

// ��������
typedef struct _PROTOCOL
{
	// �������� �̸�
	TCHAR Name[MAX_LINE_LENGTH];
	// �Ķ���� ����
	DWORD ParametersCount;
	// �Ķ���� ���� : �迭�� ��� �ִ� �Ķ���͸� ���� �� �ִ� ������ 255��
	PARAMETER Parameters[MAX_PARAMETER_COUNT];
}PROTOCOL;

int _tmain(int argc, _TCHAR* argv[])
{
	// ���� �̸� �Ķ���Ͱ� ���ٸ� �̽���
	if (argc == 1)
		return 0;

	// �������� ������ ������ PROTOCOL ����ü �迭�� ����
	PROTOCOL* Protocol = new PROTOCOL[MAX_PROTOCOL_COUNT];
	// ���� ���������� ����� �������� ����ü�� ��ġ
	DWORD ProtocolPos = 0;

	// ������ structure ���� �̸�
	TCHAR StructureFileName[MAX_PATH] = { 0, };
	// ������ Read �Լ� ���� �̸�
	TCHAR ReadPacketFileName[MAX_PATH] = { 0, };
	// ������ Write �Լ� ���� �̸�
	TCHAR WritePacketFileName[MAX_PATH] = { 0, };

	// structure ������ �ڿ� _Structure.h�� ����
	// read ������ �ڿ� _ReadPacket.h�� ����
	// write ������ �ڿ� _WritePacket.h�� ����
	_sntprintf(StructureFileName, MAX_PATH, L"%s_Structure.h", argv[2]);
	_sntprintf(ReadPacketFileName, MAX_PATH, L"%s_ReadPacket.h", argv[2]);
	_sntprintf(WritePacketFileName, MAX_PATH, L"%s_WritePacket.h", argv[2]);

	// �������� ������ ������ �޸𸮸� �ʱ�ȭ
	ZeroMemory(Protocol, sizeof(PROTOCOL) * MAX_PROTOCOL_COUNT);
	
	// ���� ���� ���⼭�� Protocol.h�� ��
	FILE* SourceFile = _tfopen(argv[1], L"r");

	// �� ������ �о ������ ����
	TCHAR Data[MAX_LINE_LENGTH] = { 0, };
	// ���� �Ķ���͸� �д� �������� Ȯ���ϴ� FLAG ����
	// �Ķ���ʹ� �׻� /* */ �ȿ� ����ֱ� ������ üũ�� �� ����
	BOOL IsParameterOpen = FALSE;
	// ���� �� ��° �Ķ���������� Ȯ���ϴ� ����
	DWORD ParameterPos = 0;

	// �� ���κ��� �о ���������� �޸𸮿� ����
	while (_fgetts(Data, MAX_LINE_LENGTH, SourceFile)) {
		// ����, ��, ���� ���� ������ �����͸� �����ϴ� ����
		TCHAR TempData[MAX_LINE_LENGTH] = { 0, };
		// ���� �а� �ִ� ������ ��ġ
		DWORD TempDataPos = 0;
		for (DWORD i = 0; i < _tcslen(Data); ++i) {
			// ���� �� ���� ���� ����, ��, ���� ���� ������
			if (Data[i] != ' ' && Data[i] != '\t' && Data[i] != '\n' && Data[i] != '\r') {
				// TempData ���ۿ� �����ؼ� �־���
				TempData[TempDataPos++] = Data[i];
			}
		}

		// �� ������ �����Ͱ� �������� �̸����� �Ķ�������� ����
		// �Ķ������ ��� ���� ���� ������ �̷����� �������� �̸��� ����
		// �Ķ���ʹ� �ִ� 255������ �� �� ����
		
		// ���� �ȿ� ','�� ������ ��������
		// enum�� Ư¡�� ','�� Ȯ��
		// �̺κ� ������ �Ķ���Ϳ��� ','�� ���� �ȵ�
		TCHAR* Pos = _tcsstr(TempData, L",");
		if (Pos) {
			// ���������� ��� VERSION�̶�� ���ڿ��� ������ ��� ����
			// �����ϰ� ���� ���� ���, �������� ��ġ�� �ϳ� �������Ѽ� �������� �̸��� ����
			if (!_tcsstr(TempData, L"VERSION")) {
				_tcsncpy(Protocol[ProtocolPos++].Name, TempData, Pos - TempData);
			}
			else {

			}
		}
		// �������� �̸��� �ƴ� ���
		// �� ��쿣 ������� �������̰ų� �Ķ���� ������ ��
		else {
			// �����Ϳ� �Ķ���� ���� ��ȣ�� ���� ���
			if (_tcsstr(TempData, L"/*")) {
				// �Ķ���� ���� FLAG�� TRUE�� ����
				IsParameterOpen = TRUE;
				// �Ķ���� ���� ������ �ʱ�ȭ
				ParameterPos = 0;
				// ���� ������ ����
				continue;
			}

			// �����Ϳ� �Ķ���� ���� ��ȣ�� ���� ���
			if (_tcsstr(TempData, L"*/")) {
				// �Ķ���� ���� FLAG�� FALSE
				IsParameterOpen = FALSE;
				// �ش� ���������� �Ķ���� ������ �Է�
				Protocol[ProtocolPos - 1].ParametersCount = ParameterPos;
				// �Ķ���� ������ �ʱ�ȭ
				ParameterPos = 0;
				// ���� ������ ����
				continue;
			}

			// �Ķ���� ���� ��ȣ�� �ݱ� ��ȣ�� �ƴ� ���
			if (_tcslen(TempData) != 0) {
				// �Ķ���Ͱ� ���� ������ ��� (�� ���� �Ķ���� �����͸� �о���� ��)
				if (IsParameterOpen) {
					// ���� ���� ���
					// �Ķ�����ε�, ��, �̸�, ����� �и�
					Pos = _tcsstr(TempData, L":");

					// ���ο� ��, ������ �����ڰ� ���� ���
					if (Pos) {
						// �Ķ���� �� ����
						_tcsncpy(Protocol[ProtocolPos - 1].Parameters[ParameterPos++].Type, 
									TempData, Pos - TempData);
					}
					// : ��ȣ�� ���� ��� ���� ������ ����
					else {
						continue;
					}
					// :�� �ִ� ���ο� [ ��ȣ�� ���� ��� �� ��ġ�� ã��
					TCHAR *StartPos = _tcsstr(TempData, L"[");
					// �� ��ġ�� 0�� �ƴ� ���
					if (StartPos) {
						// ] ��ȣ�� �ִ� ��ġ�� ã��
						TCHAR* EndPos = _tcsstr(TempData, L"]");
						TCHAR Length[MAX_LINE_LENGTH] = { 0, };

						// [���ۺ��� ]�������� ���̸� ���ؼ� ������ �Ķ���Ͱ� �迭�� ���
						// �� �迭 ũ�⸦ ���ؼ� Length�� ����
						_tcsncpy(Length, StartPos + 1, EndPos - StartPos - 1);
						// �������� ����ü�� ������
						Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Length = _tstoi(Length);

						// �Ķ���� �̸��� ����
						_tcsncpy(Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Name,
								Pos + 1, StartPos - Pos - 1);
					}
					else {
						// ���� [ ��ȣ�� ���� ���� �迭���� �ƴϱ� ������ ���̸� 0���� ����
						Protocol[ProtocolPos - 1].Parameters[ParameterPos - 1].Length = 0;
					}
				}
			}
		}
	}

	fclose(SourceFile);

	// �� ������ ��ġ�� �Ǹ� Protocol.h ������ �о
	// �� ������ �������� �迭�� �Ķ���� ���� �����ؼ� ����
	// �Ʒ� ������ʹ� ����� �������� �迭���� Output ���ϵ��� ������ ���� ������ ������

	// structure �����
	FILE* StructureFile = _tfopen(StructureFileName, L"w");

	_ftprintf(StructureFile, L"pragma once\n\n");

	// ���� �������� ��ü ������ŭ �ݺ���
	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// ���� �������ݰ� �Ķ���� ������ �̿��ؼ� ����ü�� ����
		_ftprintf(StructureFile, L"typedef struct _S_%s\n{\n", Protocol[i].Name);
		// ���� ���������� �Ķ���� ������ŭ �ݺ�
		for (DWORD j = 0; j < Protocol[i].Parameters[j].Length; ++j) {
			// �迭���� �ƴ� �Ķ������ ���
			if (Protocol[i].Parameters[j].Length == 0) {
				// ������ ���� �������� �� ��
				_ftprintf(StructureFile, L"\t%s %s;\n",
				Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name);
			}
			// �迭���� �Ķ������ ���
			else {
				// ��������, ������, �迭 ũ�⸦ ����
				_ftprintf(StructureFile, L"\t%s %s[%d];",
					Protocol[i].Parameters[j].Type, Protocol[i].Parameters[j].Name,
					Protocol[i].Parameters[j].Length);
			}
		}
		// ���������� ����ü�� �ݰ� �̸��� ����
		_ftprintf(StructureFile, L"} S_%s;\n\n", Protocol[i].Name);
	}

	fclose(StructureFile);
	// ���� �۾��� �ݺ��ϰ� �Ǹ� ��� �������ݵ��� �Ķ���͸� ������ ����ü�� ����

	// Read �Լ� �����
	FILE* ReadPacketFile = _tfopen(ReadPacketFileName, L"w");

	_ftprintf(ReadPacketFile, L"#pragma once\n\n");

	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// READ �Լ� ����, inline ���·� ����
		// �� READ �Լ����� ������ �̸� ����� ���� ������ �̿��ؼ� �����͸� �а� ��
		_ftprintf(ReadPacketFile, L"inline VOID READ_%s(BYTE* buffer, S%s &parameter", 
			Protocol[i].Name, Protocol[i].Name);
		// ���̳ʸ����� �����͸� �б� ���ؼ� StreamSP Ŭ������ ���
		_ftprintf(ReadPacketFile, L"\tStreamSP Stream;\n");
		_ftprintf(ReadPacketFile, L"\tStream->SetBuffer(buffer);\n\n");

		// �ش� ���������� �Ķ���� ���� �ݺ�
		for (DWORD j = 0; j < Protocol[i].ParametersCount; ++j) {
			// �Ķ���� ���� INT ��
			if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT")) {
				// ReadInt32 �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadInt32(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� DWORD ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD")) {
				// ReadDWORD �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadDWORD(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� DWORD_PTR ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD_PTR")) {
				// ReadDWORD_PTR �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadDWORD_PTR(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� BYTE ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BYTE")) {
				// ���� �Ķ������ ���̰� 0�̸�
				if (Protocol[i].Parameters[j].Length == 0) {
					// ReadByte �Լ��� ���� 1����Ʈ�� ����ü�� �Է�
					_ftprintf(ReadPacketFile, L"\tStream->ReadByte(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// �Ķ������ ���̰� 0���� Ŭ ���, �� �迭�� ���
				else {
					// ReadBytes �Լ��� �̿��ؼ� ���۸� ����
					_ftprintf(ReadPacketFile, L"\tStream->ReadBytes(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// �Ķ���� ���� WCHAR ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"WCHAR")) {
				// ���� �Ķ������ ���̰� 0�̸�
				if (Protocol[i].Parameters[j].Length == 0) {
					// ReadWCHAR �Լ��� ���� 1����Ʈ�� ����ü�� �Է�
					_ftprintf(ReadPacketFile, L"\tStream->ReadWCHAR(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// �Ķ������ ���̰� 0���� Ŭ ���, �� �迭�� ���
				else {
					// ReadWCHARs �Լ��� �̿��ؼ� ���۸� ����
					_ftprintf(ReadPacketFile, L"\tStream->ReadWCHARs(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// �Ķ���� ���� FLOAT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"FLOAT")) {
				// ReadFloat �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadFloat(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� INT64 ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT64")) {
				// ReadInt64 �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadInt64(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� USHORT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"USHORT")) {
				// ReadUSHORT �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadUSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� SHORT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"SHORT")) {
				// ReadSHORT �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� BOOL ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BOOL")) {
				// ReadBOOL �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->ReadBOOL(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �� ���� ���� BYTE�� �̿��ؼ� ����ü�� �Է�
			else {
				_ftprintf(ReadPacketFile, L"\tStream->ReadBytes((BYTE*)parameter.%s, sizeof(%s) * %d);\n",
					Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type, 
					Protocol[i].Parameters[j].Length);
			}
		}
		_ftprintf(ReadPacketFile, L"}\n\n");
	}

	fclose(ReadPacketFile);
	// �� �κ��� �����ϸ� Read �Լ����� ������ ���� ������ �ϳ� ����

	// �������� ���캼 Write �Լ� ������ 2������ Write �Լ��� ����
	// ����ü�� �̿��� Write �Լ��� �Ķ���͸� �ϳ��� �Է��ϴ� �Ķ���� Ÿ���� ����
	// ����ü Ÿ���� Write �Լ� �����
	FILE* WritePacketFile = _tfopen(WritePacketFileName, L"w");

	_ftprintf(WritePacketFile, L"#pragma once\n\n");

	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// WRITE �Լ� ����, inline ���·� ����
		// �� WRITE �Լ����� ������ �̸� ����� ���� ������ �̿��ؼ� �����͸� �а� ��
		_ftprintf(WritePacketFile, L"inline DWORD WRITE_%s(BYTE* buffer, S%s &parameter",
			Protocol[i].Name, Protocol[i].Name);
		// ���̳ʸ����� �����͸� �б� ���ؼ� StreamSP Ŭ������ ���
		_ftprintf(WritePacketFile, L"\tStreamSP Stream;\n");
		_ftprintf(WritePacketFile, L"\tStream->SetBuffer(buffer);\n\n");

		// �ش� ���������� �Ķ���� ���� �ݺ�
		for (DWORD j = 0; j < Protocol[i].ParametersCount; ++j) {
			// �Ķ���� ���� INT ��
			if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT")) {
				// WriteInt32 �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteInt32(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� DWORD ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD")) {
				// WriteDWORD �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteDWORD(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� DWORD_PTR ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"DWORD_PTR")) {
				// WriteDWORD_PTR �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteDWORD_PTR(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� BYTE ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BYTE")) {
				// ���� �Ķ������ ���̰� 0�̸�
				if (Protocol[i].Parameters[j].Length == 0) {
					// WriteByte �Լ��� ���� 1����Ʈ�� ����ü�� �Է�
					_ftprintf(WritePacketFile, L"\tStream->WriteByte(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// �Ķ������ ���̰� 0���� Ŭ ���, �� �迭�� ���
				else {
					// WriteBytes �Լ��� �̿��ؼ� ���۸� ����
					_ftprintf(WritePacketFile, L"\tStream->WriteBytes(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// �Ķ���� ���� WCHAR ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"WCHAR")) {
				// ���� �Ķ������ ���̰� 0�̸�
				if (Protocol[i].Parameters[j].Length == 0) {
					// WriteWCHAR �Լ��� ���� 1����Ʈ�� ����ü�� �Է�
					_ftprintf(WritePacketFile, L"\tStream->WriteWCHAR(&parameter.%s);\n",
						Protocol[i].Parameters[j].Name);
				}
				// �Ķ������ ���̰� 0���� Ŭ ���, �� �迭�� ���
				else {
					// WriteWCHARs �Լ��� �̿��ؼ� ���۸� ����
					_ftprintf(WritePacketFile, L"\tStream->WriteWCHARs(parameter.%s, %d);\n",
						Protocol[i].Parameters[j].Name,
						Protocol[i].Parameters[j].Length);
				}
			}
			// �Ķ���� ���� FLOAT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"FLOAT")) {
				// WriteFloat �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteFloat(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� INT64 ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"INT64")) {
				// WriteInt64 �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->WriteInt64(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� USHORT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"USHORT")) {
				// WriteUSHORT �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteUSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� SHORT ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"SHORT")) {
				// WriteSHORT �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(WritePacketFile, L"\tStream->WriteSHORT(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �Ķ���� ���� BOOL ��
			else if (!_tcscmp(Protocol[i].Parameters[j].Type, L"BOOL")) {
				// WriteBOOL �Լ��� ���� �����͸� ����ü�� �Է�
				_ftprintf(ReadPacketFile, L"\tStream->WriteBOOL(&parameter.%s);\n",
					Protocol[i].Parameters[j].Name);
			}
			// �� ���� ���� BYTE�� �̿��ؼ� ����ü�� �Է�
			else {
				_ftprintf(WritePacketFile, L"\tStream->WriteBytes((BYTE*)parameter.%s, sizeof(%s) * %d);\n",
					Protocol[i].Parameters[j].Name, Protocol[i].Parameters[j].Type,
					Protocol[i].Parameters[j].Length);
			}
		}
		_ftprintf(WritePacketFile, L"\n\treturn Stream->GetLength();\n}\n\n");
	}

	// ���� �Լ� �̸��� �Ķ���͸� �����ؼ� �Ķ���� Ÿ�� �Լ��� ����
	// Write �Լ� �Ķ���� Ÿ��
	// ��ü �������� ������ŭ �ݺ�
	for (DWORD i = 0; i < ProtocolPos; ++i) {
		// Write �Լ��� ������ ����
		// WRITE �Լ� ����, inline ���·� ����
		// �� WRITE �Լ����� ������ �̸� ����� ���� ������ �̿��ؼ� �����͸� �а� ��
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
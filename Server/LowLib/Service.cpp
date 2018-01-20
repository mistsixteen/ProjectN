#include "stdafx.h"
#include "Service.h"

Service* service = NULL;

VOID WINAPI RunCallback(DWORD argumentCount, LPTSTR * arguments)
{
	service->RunCallback(argumentCount, arguments);
}

VOID WINAPI CtrlHandlerCallback(DWORD opCode)
{
	service->CtrlHandlerCallback(opCode);
}

// ���� ���
BOOL Service::Install(LPCTSTR serviceName)		// �Ķ���ͷ� ���� �̸��� ����
{
	TCHAR		ServiceFileName[MAX_PATH]	= { 0, };
	SC_HANDLE	ServiceControlManager		= NULL;
	SC_HANDLE	ServiceHandle				= NULL;

	if (!serviceName)
		return FALSE;

	// ���� ���� �̸��� �޾ƿ�
	ServiceFileName[0] = '"';
	GetModuleFileName(NULL, ServiceFileName + 1, MAX_PATH - 1);
	_tcscat(ServiceFileName, L"\"");

	// ���� ���� �ɼ����� SCM�� Open
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
		return FALSE;

	// ���񽺸� SERVICE_WIN32_OWN_PROCESS ���·� ����
	ServiceHandle = CreateService(ServiceControlManager,
		serviceName,					// ���� �̸�
		serviceName,					// �������� �̸��� ����
		SERVICE_ALL_ACCESS,				// ��� ������ ����
		SERVICE_WIN32_OWN_PROCESS,		// Win32 ������ ����
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		ServiceFileName,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	// ���� ���� �� �Ѿ
	if (!ServiceHandle) {
		// ����
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

// ���� ����
BOOL Service::Uninstall(LPCTSTR serviceName)
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE	ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// �ν��� �� ���� �����ϰ� SCManager Open
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
		return FALSE;

	// DELETE�� ���� OpenService
	// DELETE �ɼ����� OpenService ȣ��
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	// ���� �� ����
	if (!ServiceHandle) {
		// ����
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	// ���� ���񽺸� ����
	if (!DeleteService(ServiceHandle)) {
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);
		
		return FALSE;
	}

	// �Ϸ� �� ����� �ڵ� ���� �ݾ���
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

// ���� ������ ���۵� �� ���
BOOL Service::Begin(LPTSTR serviceName)
{
	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher���� ����� ���� ȯ�� ����
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{serviceName, ::RunCallback},
		// ���� �̸��� ���񽺰� ����Ǿ��� �� ȣ��� �ݹ�(RunCallback)
		{NULL, NULL}
	};

	// ������ ��� ���� �̸��� ����ϹǷ� this->serviceName �� ����
	_tcscpy(this->serviceName, serviceName);

	OnStarted();			// ���� �Լ� OnStarted ȣ��

	if (!StartServiceCtrlDispatcher(DispatchTable)) {
		// ����
		_tprintf(L"## Debug console mode ##\n");
		getchar();
	}

	return TRUE;
}

BOOL Service::End(VOID)
{
	OnStopped();

	return TRUE;
}

// ���� ���� �ݹ� StartServiceCtrlDispatcher �Լ��� ���������� ���� �� �Ѿ���� �ݹ�
// �� �Լ� ������ ������ ����
VOID Service::RunCallback(DWORD argumentCount, LPTSTR* arguments)
{
	DWORD Status;

	// ���� ����ϴ� ���� �����̹Ƿ� ����� this->ServiceStatus�� ���� ���� ����Ͽ� ���
	this->serviceStatus.dwServiceType = SERVICE_WIN32;
	this->serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	this->serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	this->serviceStatus.dwWin32ExitCode = 0;
	this->serviceStatus.dwServiceSpecificExitCode = 0;
	this->serviceStatus.dwCheckPoint = 0;
	this->serviceStatus.dwWaitHint = 0;

	// ���¸� SERVICE_START_PENDING���� �س��� Control Callback�� ���
	this->serviceStatusHandle = RegisterServiceCtrlHandler(this->serviceName, ::CtrlHandlerCallback);

	// ��� ���� �� ����
	if (this->serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	Status = NO_ERROR;

	// ��� ���� �� SERVICE_RUNNING ���·� ����
	this->serviceStatus.dwCurrentState = SERVICE_RUNNING;
	this->serviceStatus.dwCheckPoint = 0;
	this->serviceStatus.dwWaitHint = 0;

	if (!SetServiceStatus(this->serviceStatusHandle, &this->serviceStatus))
		// ���� ���� ����
		return;
}

// ���� ���� �ݹ�
VOID Service::CtrlHandlerCallback(DWORD opCode)
{
	switch (opCode)
	{
	case SERVICE_CONTROL_PAUSE:		// ��� ���� ����
		this->serviceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:	// Continue ����
		this->serviceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:		// ���� ����
		// ���� ���� �Լ� OnStopped�� �־�� �ϴ� ��ġ
		this->serviceStatus.dwWin32ExitCode	= 0;
		this->serviceStatus.dwCurrentState	= SERVICE_STOPPED;
		this->serviceStatus.dwWin32ExitCode = 0;
		this->serviceStatus.dwWin32ExitCode = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}

	// ���� ������ �Ǿ����Ƿ� ����
	if (!SetServiceStatus(this->serviceStatusHandle, &this->serviceStatus))
		return;
}

Service::Service()
{
	ZeroMemory(serviceName, sizeof(serviceName));

	service = this;
}

Service::~Service()
{
}
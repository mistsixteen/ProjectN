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

// 서비스 등록
BOOL Service::Install(LPCTSTR serviceName)		// 파라미터로 서비스 이름을 받음
{
	TCHAR		ServiceFileName[MAX_PATH]	= { 0, };
	SC_HANDLE	ServiceControlManager		= NULL;
	SC_HANDLE	ServiceHandle				= NULL;

	if (!serviceName)
		return FALSE;

	// 실행 파일 이름을 받아옴
	ServiceFileName[0] = '"';
	GetModuleFileName(NULL, ServiceFileName + 1, MAX_PATH - 1);
	_tcscat(ServiceFileName, L"\"");

	// 서비스 생성 옵션으로 SCM을 Open
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
		return FALSE;

	// 서비스를 SERVICE_WIN32_OWN_PROCESS 형태로 생성
	ServiceHandle = CreateService(ServiceControlManager,
		serviceName,					// 서비스 이름
		serviceName,					// 보여지는 이름은 동일
		SERVICE_ALL_ACCESS,				// 모든 엑세스 가능
		SERVICE_WIN32_OWN_PROCESS,		// Win32 형태의 파일
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		ServiceFileName,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	// 생성 성공 시 넘어감
	if (!ServiceHandle) {
		// 실패
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

// 서비스 삭제
BOOL Service::Uninstall(LPCTSTR serviceName)
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE	ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// 인스톨 할 때와 동일하게 SCManager Open
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
		return FALSE;

	// DELETE를 위한 OpenService
	// DELETE 옵션으로 OpenService 호출
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	// 실패 시 종료
	if (!ServiceHandle) {
		// 실패
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	// 실제 서비스를 삭제
	if (!DeleteService(ServiceHandle)) {
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);
		
		return FALSE;
	}

	// 완료 후 사용한 핸들 값을 닫아줌
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

// 실제 서버가 시작될 때 사용
BOOL Service::Begin(LPTSTR serviceName)
{
	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher에서 등록할 서비스 환경 정보
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		{serviceName, ::RunCallback},
		// 서비스 이름과 서비스가 실행되었을 때 호출될 콜백(RunCallback)
		{NULL, NULL}
	};

	// 앞으로 계속 서비스 이름을 사용하므로 this->serviceName 내 복제
	_tcscpy(this->serviceName, serviceName);

	OnStarted();			// 가상 함수 OnStarted 호출

	if (!StartServiceCtrlDispatcher(DispatchTable)) {
		// 실패
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

// 서비스 시작 콜백 StartServiceCtrlDispatcher 함수가 성공적으로 실행 시 넘어오는 콜백
// 이 함수 원형은 정해져 있음
VOID Service::RunCallback(DWORD argumentCount, LPTSTR* arguments)
{
	DWORD Status;

	// 현재 사용하는 서비스 상태이므로 멤버인 this->ServiceStatus에 값을 직접 등록하여 사용
	this->serviceStatus.dwServiceType = SERVICE_WIN32;
	this->serviceStatus.dwCurrentState = SERVICE_START_PENDING;
	this->serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	this->serviceStatus.dwWin32ExitCode = 0;
	this->serviceStatus.dwServiceSpecificExitCode = 0;
	this->serviceStatus.dwCheckPoint = 0;
	this->serviceStatus.dwWaitHint = 0;

	// 상태를 SERVICE_START_PENDING으로 해놓고 Control Callback을 등록
	this->serviceStatusHandle = RegisterServiceCtrlHandler(this->serviceName, ::CtrlHandlerCallback);

	// 등록 실패 시 종료
	if (this->serviceStatusHandle == (SERVICE_STATUS_HANDLE)0)
		return;

	Status = NO_ERROR;

	// 등록 성공 시 SERVICE_RUNNING 상태로 변경
	this->serviceStatus.dwCurrentState = SERVICE_RUNNING;
	this->serviceStatus.dwCheckPoint = 0;
	this->serviceStatus.dwWaitHint = 0;

	if (!SetServiceStatus(this->serviceStatusHandle, &this->serviceStatus))
		// 서비스 상태 변경
		return;
}

// 서비스 상태 콜백
VOID Service::CtrlHandlerCallback(DWORD opCode)
{
	switch (opCode)
	{
	case SERVICE_CONTROL_PAUSE:		// 잠시 멈춤 상태
		this->serviceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:	// Continue 상태
		this->serviceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:		// 멈춤 상태
		// 원래 가상 함수 OnStopped가 있어야 하는 위치
		this->serviceStatus.dwWin32ExitCode	= 0;
		this->serviceStatus.dwCurrentState	= SERVICE_STOPPED;
		this->serviceStatus.dwWin32ExitCode = 0;
		this->serviceStatus.dwWin32ExitCode = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	}

	// 상태 변경이 되었으므로 적용
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
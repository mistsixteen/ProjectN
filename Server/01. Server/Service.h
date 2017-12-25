#pragma once

class Service
{
private:
	TCHAR					serviceName[MAX_PATH];
	SERVICE_STATUS			serviceStatus;
	SERVICE_STATUS_HANDLE	serviceStatusHandle;
public:
	BOOL Install(LPCTSTR serviceName);
	BOOL Uninstall(LPCTSTR serviceName);

	BOOL Begin(LPTSTR serviceName);
	BOOL End(VOID);

	VOID RunCallback(DWORD argumentCount, LPTSTR* arguments);
	VOID CtrlHandlerCallback(DWORD opCode);
protected:
	virtual VOID OnStarted() PURE;
	virtual VOID OnStopped() PURE;
public:
	Service();
	~Service();
};


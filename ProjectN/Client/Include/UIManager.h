#pragma once
#include "UI.h"

class UI;
class UIManager
{
	DECLARE_SINGLETON(UIManager);
private:
	map<const TCHAR*, list<UI*>>	UIMap;
public:
	void AddUI(const TCHAR* key, UI* ui);
	void Progress(void);
	void Render(void);
	void Release(void);
private:
	UIManager();
public:
	
	~UIManager();
};


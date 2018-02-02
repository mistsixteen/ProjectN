#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager()
{
}


UIManager::~UIManager()
{
	Release();
}

void UIManager::AddUI(const TCHAR * key, UI * ui)
{
	UIMap[key].push_back(ui);
}

void UIManager::Progress(void)
{
	for (auto iterMap = UIMap.begin(); iterMap != UIMap.end(); ++iterMap){
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList) {
			(*iterList)->Progress();
		}
	}
}

void UIManager::Render(void)
{
	for (auto iterMap = UIMap.begin(); iterMap != UIMap.end(); ++iterMap) {
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList) {
			(*iterList)->Render();
		}
	}
}

void UIManager::Release(void)
{
	for (auto iterMap = UIMap.begin(); iterMap != UIMap.end(); ++iterMap)
	{
		for (auto iterList = (*iterMap).second.begin();
			iterList != (*iterMap).second.end(); ++iterList)
		{
			SAFE_DELETE(*iterList);
		}
		(*iterMap).second.clear();
	}
	UIMap.clear();
}


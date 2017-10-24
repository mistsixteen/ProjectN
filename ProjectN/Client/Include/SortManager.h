#pragma once
#include "Define.h"
#include "GameObject.h"

class GameObject;
class SortManager
{
	DECLARE_SINGLETON(SortManager)
private:
	vector<GameObject*>	sortVector;
public:
	void Insert(GameObject* object);
	void Render();
	void Release();
private:
	SortManager();
public:
	~SortManager();
};


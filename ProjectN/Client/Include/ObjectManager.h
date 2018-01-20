#pragma once
#include "Define.h"
#include "GameObject.h"

#include "SortManager.h"

class GameObject;
class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)
private:
	map<const TCHAR*, list<GameObject*>> objectMap;
public:
	void AddObject(const TCHAR* key, GameObject* object);
	void Progress(void);
	void Render(void);
	void Release(void);
public:
	map<const TCHAR*, list<GameObject*>>& GetObjectMap() { return objectMap; };
	GameObject* GetGameObject(const TCHAR* key, int count = 0);
	float GetTerrainHeight(INFO srcInfo);
private:
	ObjectManager();
public:
	~ObjectManager();
};


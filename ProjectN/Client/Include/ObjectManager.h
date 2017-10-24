#pragma once
#include "Define.h"
#include "GameObject.h"
#include "Math.h"

#include "SortManager.h"

class GameObject;
class ObjectManager
{
	DECLARE_SINGLETON(ObjectManager)
private:
	map<const TCHAR*, list<GameObject*>> objectMap;
public:
	void InsertObject(const TCHAR* objectKey, GameObject* object);
	void Progress(void);
	void Render(void);
	void Release(void);
public:
	GameObject* GetGameObject(const TCHAR* objectKey, int count = 0);
	float GetTerrainHeight(INFO srcInfo);
private:
	ObjectManager();
public:
	~ObjectManager();
};


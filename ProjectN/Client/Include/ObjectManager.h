#pragma once
#include "Define.h"
#include "GameObject.h"

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
	void ColisionCheck(GameObject* object, D3DXVECTOR3* nextpos, BOOL* ismoveable);
public:
	GameObject* GetGameObject(const TCHAR* objectKey, int count = 0);
private:
	ObjectManager();
public:
	~ObjectManager();
};


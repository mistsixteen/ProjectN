#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"

class Factory
{
public:
	static GameObject* CreateInstance(const TCHAR* factoryKey, const TCHAR* key, INFO info)
	{
		if (_tcscmp(factoryKey, L"Terrain") == 0)
		{
			GameObject* object = new Terrain(key, info);
			if (FAILED(object->Initialize()))
			{
				SAFE_DELETE(object);
				return NULL;
			}
			return object;
		}
		else if (_tcscmp(factoryKey, L"Player") == 0)
		{
			GameObject* object = new Player(key, info);
			if (FAILED(object->Initialize()))
			{
				SAFE_DELETE(object);
				return NULL;
			}
			return object;
		}
		else
			return NULL;
	}
private:
	Factory() {};
public:
	~Factory() {};
};

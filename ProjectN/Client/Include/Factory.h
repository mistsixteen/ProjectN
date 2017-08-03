#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Terrain.h"

class Factory
{
public:
	static GameObject* CreateInstance(const TCHAR* objectKey, const TCHAR* key)
	{
		if (_tcscmp(objectKey, L"Terrain") == 0)
		{
			GameObject* object = new Terrain(key);
			if (FAILED(object->Initialize()))
				return NULL;
			return object;
		}

		return NULL;
	}
private:
	Factory() {};
public:
	~Factory() {};
};

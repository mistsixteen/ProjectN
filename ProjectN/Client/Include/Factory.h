#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Player.h"
#include "Background.h"

template<typename T>
class Factory
{
public:
	static GameObject* CreateInstance(const TCHAR* key, INFO info)
	{
		GameObject* object = new T(key, info);
		if (FAILED(object->Initialize()))
		{
			SAFE_DELETE(object);
			return NULL;
		}
		return object;
	}
private:
	Factory() {};
public:
	~Factory() {};
};

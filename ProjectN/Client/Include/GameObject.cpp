#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
	:key(NULL)
{
}

GameObject::GameObject(const TCHAR * _key, INFO _information)
	: key(_key), information(_information)
{
	information.direction.x = D3DXToRadian((int)information.direction.x % 360);
	information.direction.y = D3DXToRadian((int)information.direction.y % 360);
	information.direction.z = D3DXToRadian((int)information.direction.z % 360);
}

GameObject::~GameObject()
{
}

#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
	:key(NULL)
{
}

GameObject::GameObject(const TCHAR * _key)
	:key(_key)
{
}


GameObject::~GameObject()
{
}

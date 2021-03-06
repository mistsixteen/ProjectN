#include "stdafx.h"
#include "GameObject.h"

void GameObject::Render(void)
{
	D3DXMatrixRotationX(&rotX, information.direction.x);
	D3DXMatrixRotationY(&rotY, information.direction.y);
	D3DXMatrixRotationZ(&rotZ, information.direction.z);
	D3DXMatrixTranslation(&trans, information.position.x, information.position.y, information.position.z);
	world = scale * rotX * rotY * rotZ * trans;
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_WORLD, &world);
}

GameObject::GameObject()
	:key(NULL)
{
}

GameObject::GameObject(const TCHAR * _key, INFO _information)
	: key(_key), information(_information)
{
	/*information.direction.x = D3DXToRadian((int)information.direction.x % 360);
	information.direction.y = D3DXToRadian((int)information.direction.y % 360);
	information.direction.z = D3DXToRadian((int)information.direction.z % 360);*/
}

GameObject::~GameObject()
{
}

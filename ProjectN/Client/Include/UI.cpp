#include "stdafx.h"
#include "UI.h"

UI::UI()
	:key(NULL)
{
}

UI::UI(const TCHAR * _key)
	:key(_key)
{
}


UI::~UI()
{
}

void UI::Render(void)
{
	D3DXMatrixRotationX(&rotX, information.direction.x);
	D3DXMatrixRotationY(&rotY, information.direction.y);
	D3DXMatrixRotationZ(&rotZ, information.direction.z);
	D3DXMatrixTranslation(&trans, information.position.x, information.position.y, information.position.z);
	world = scale * rotX * rotY * rotZ * trans;
	GET_SINGLE(DXFramework)->GetDevice()->SetTransform(D3DTS_WORLD, &world);
}


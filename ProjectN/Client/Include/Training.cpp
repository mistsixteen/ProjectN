#include "stdafx.h"
#include "Training.h"

Training::Training()
{
}

Training::~Training()
{
	Release();
}

HRESULT Training::Initialize()
{
	return S_OK;
}

void Training::Progress()
{
	GET_SINGLE(ObjectManager)->Progress();
}

void Training::Render()
{
	GET_SINGLE(ObjectManager)->Render();
}

void Training::Release()
{
	GET_SINGLE(ObjectManager)->Release();
}


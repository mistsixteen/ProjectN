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
	GET_SINGLE(ObjectFactory)->Progress();
}

void Training::Render()
{
	GET_SINGLE(ObjectFactory)->Render();
}

void Training::Release()
{
	GET_SINGLE(ObjectFactory)->Release();
}


#include "stdafx.h"
#include "Training.h"

CTraining::CTraining()
{
}

CTraining::~CTraining()
{
	Release();
}

HRESULT CTraining::Initialize()
{
	return S_OK;
}

void CTraining::Progress()
{
	GET_SINGLE(CObjMgr)->Progress();
}

void CTraining::Render()
{
	GET_SINGLE(CObjMgr)->Render();
}

void CTraining::Release()
{
	GET_SINGLE(CObjMgr)->Release();
}


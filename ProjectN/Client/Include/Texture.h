#pragma once
#include "Define.h"
#include "DXFramework.h"

class Texture
{
public:
	virtual const TEXINFO*	GetTexture(const TCHAR*	state = NULL, const int& count = 0)PURE;
public:
	virtual HRESULT	AddTexture(const TCHAR* fileName, TEXTYPE type,	
									const TCHAR* state = NULL, const int& count = 0)PURE;
	virtual void Release(void) PURE;
public:
	Texture();
	virtual ~Texture();
};


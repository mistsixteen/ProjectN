#pragma once
#include "Texture.h"

class MultiTexture :
	public Texture
{
private:
	map<const TCHAR*, vector<TEXINFO*>> texMap;
public:	
	virtual const TEXINFO*	GetTexture(const TCHAR*	state = NULL, const int& count = 0);
public:
	virtual HRESULT	AddTexture(const TCHAR* path, TEXTYPE type,
								const TCHAR* state = 0, const int& count = 0);
	virtual void Release(void);
public:
	MultiTexture();
	~MultiTexture();
};


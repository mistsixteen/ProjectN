#pragma once

#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

class Texture;
class TextureManager
{
	DECLARE_SINGLETON(TextureManager)
private:
	map<const TCHAR*, Texture*> texMap;
public:
	const TEXINFO* GetTexture(const TCHAR* key, const TCHAR* state, const int& count = 0);
public:
	HRESULT AddTexture(const TCHAR* path, TEXTYPE type,  const TCHAR* key
						,const TCHAR* state, const int& count = 0);
	void Release();
private:
	TextureManager();
public:
	~TextureManager();
};


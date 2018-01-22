#include "stdafx.h"
#include "TextureManager.h"


const TEXINFO * TextureManager::GetTexture(const TCHAR * key, const TCHAR * state, const int & count)
{
	auto iterMap = texMap.find(key);
	if (iterMap == texMap.end())
		return nullptr;

	return iterMap->second->GetTexture(state, count);
}

HRESULT TextureManager::AddTexture(const TCHAR * path, TEXTYPE type, const TCHAR * key, const TCHAR * state, const int & count)
{
	auto iterMap = texMap.find(key);
	if (iterMap == texMap.end()) {
		Texture* texture = NULL;
		if (count == 0) {
			texture = new SingleTexture;
		}
		else {
			texture = new MultiTexture;
		}


		if (FAILED(texture->AddTexture(path, type, state, count))) {
			return E_FAIL;
		}
		texMap.insert(make_pair(key, texture));
	}
	else
		return E_FAIL;

	return S_OK;
}

void TextureManager::Release()
{
	for (auto iterMap = texMap.begin(); iterMap != texMap.end(); ++iterMap) {
		SAFE_DELETE(iterMap->second);
	}
	texMap.clear();
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
	Release();
}

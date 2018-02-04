#include "stdafx.h"
#include "SingleTexture.h"


const TEXINFO * SingleTexture::GetTexture(const TCHAR * state, const int & count)
{
	auto iter = texMap.find(state);
	if (iter == texMap.end())
		return nullptr;

	return iter->second;
}

HRESULT SingleTexture::AddTexture(const TCHAR * path, TEXTYPE type, const TCHAR * state, const int & count)
{
	TEXINFO* texInfo = new TEXINFO;
	ZeroMemory(texInfo, sizeof(TEXINFO));
	if (FAILED(D3DXGetImageInfoFromFile(path, &texInfo->information))){
		MSGBOX(L"이미지 불러오기 실패");
		return E_FAIL;
	}

	if (type == TEXTYPE_GENERAL)
	{
		if (FAILED(D3DXCreateTextureFromFileEx(GET_SINGLE(DXFramework)->GetDevice()
			, path, texInfo->information.Width, texInfo->information.Height
			, texInfo->information.MipLevels, 0, texInfo->information.Format
			, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT
			, D3DCOLOR_ARGB(255, 255, 255, 255)
			, &texInfo->information, NULL, &texInfo->texture))) {
			MSGBOX(L"텍스쳐 생성 실패");
			return E_FAIL;
		}
	}
	else if (type == TEXTYPE_CUBE)
	{
		if (FAILED(D3DXCreateCubeTextureFromFile(GET_SINGLE(DXFramework)->GetDevice()
			, path, &texInfo->cubeTexture)))
		{
			MSGBOX(L"큐브 텍스쳐 생성 실패");
			return E_FAIL;

		}
	}

	texMap.insert(make_pair(state, texInfo));
	return S_OK;
}

void SingleTexture::Release(void)
{
	for (auto iterMap = texMap.begin(); iterMap != texMap.end(); ++iterMap) {
		SAFE_RELEASE(iterMap->second->texture);
		SAFE_RELEASE(iterMap->second->cubeTexture);
		SAFE_DELETE(iterMap->second);
	}
	texMap.clear();
}

SingleTexture::SingleTexture()
{
}


SingleTexture::~SingleTexture()
{
	Release();
}

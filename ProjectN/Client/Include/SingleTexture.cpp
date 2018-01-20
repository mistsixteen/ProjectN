#include "stdafx.h"
#include "SingleTexture.h"


const TEXINFO * SingleTexture::GetTexture(const TCHAR * state, const int & count)
{
	return texInfo;
}

HRESULT SingleTexture::AddTexture(const TCHAR * path, TEXTYPE type, const TCHAR * state, const int & count)
{
	texInfo = new TEXINFO;
	ZeroMemory(texInfo, sizeof(TEXINFO));
	if (FAILED(D3DXGetImageInfoFromFile(path, &texInfo->information))){
		MSGBOX(L"이미지 불러오기 실패");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(GET_SINGLE(DXFramework)->GetDevice()
		, path, texInfo->information.Width, texInfo->information.Height
		, texInfo->information.MipLevels, 0, texInfo->information.Format
		, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT
		, D3DCOLOR_ARGB(255, 255, 255, 255)
		, &texInfo->information, NULL, &texInfo->texture))) {
		MSGBOX(L"텍스쳐 생성 실패");
		return E_FAIL;
	}
	return S_OK;
}

void SingleTexture::Release(void)
{
	if (texInfo)
	{
		texInfo->texture->Release();
		delete texInfo;
		texInfo = NULL;
	}
}

SingleTexture::SingleTexture()
	:texInfo(NULL)
{
}


SingleTexture::~SingleTexture()
{
	Release();
}

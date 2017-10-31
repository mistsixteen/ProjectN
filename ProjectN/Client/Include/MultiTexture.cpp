#include "stdafx.h"
#include "MultiTexture.h"


const TEXINFO * MultiTexture::GetTexture(const TCHAR * state, const int & count)
{
	auto iter = texMap.find(state);
	if (iter == texMap.end())
		return nullptr;

	return iter->second[count];
}

HRESULT MultiTexture::AddTexture(const TCHAR * path, TEXTYPE type, const TCHAR * state, const int & count)
{
	TCHAR szPath[128] = L"";
	vector<TEXINFO*> vecTex;
	for (int i = 0; i < count; ++i) {
		wsprintf(szPath, path, i);
		
		TEXINFO* texInfo = new TEXINFO;
		ZeroMemory(texInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szPath, &texInfo->information))) {
			MSGBOX(L"이미지 불러오기 실패");
			return E_FAIL;
		}

		if (type == TEXTYPE_GENERAL)
		{
			if (FAILED(D3DXCreateTextureFromFileEx(GET_SINGLE(DXFramework)->GetDevice()
				, szPath, texInfo->information.Width, texInfo->information.Height
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
				, szPath, &texInfo->cubeTexture)))
			{
				MSGBOX(L"큐브 텍스쳐 생성 실패");
				return E_FAIL;

			}
		}
		vecTex.push_back(texInfo);
	}

	texMap.insert(make_pair(state, vecTex));

	return S_OK;
}

void MultiTexture::Release(void)
{
	for (auto iterMap = texMap.begin(); iterMap != texMap.end(); ++iterMap) {
		for (size_t i = 0; i < iterMap->second.size(); ++i) {
			SAFE_RELEASE(iterMap->second[i]->texture);
			SAFE_RELEASE(iterMap->second[i]->cubeTexture);
			SAFE_DELETE(iterMap->second[i]);
		}
		iterMap->second.clear();
	}
	texMap.clear();
}

MultiTexture::MultiTexture()
{
}


MultiTexture::~MultiTexture()
{
	Release();
}

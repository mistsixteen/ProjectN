#include "stdafx.h"
#include "ShaderManager.h"


HRESULT ShaderManager::AddShader(const TCHAR * shaderKey, const TCHAR * fileName)
{
	auto iter = ShaderMap.find(shaderKey);
	if (iter != ShaderMap.end())
	{
		MSGBOX(L"이미 존재하는 셰이더 키입니다.");
		return E_FAIL;
	}

	Shader* pShader = new Shader;
	pShader->InitShader(fileName);
	ShaderMap.insert(make_pair(shaderKey, pShader));

	return S_OK;
}

LPD3DXEFFECT ShaderManager::GetShader(const TCHAR * shaderKey)
{
	auto iter = ShaderMap.find(shaderKey);
	if (iter == ShaderMap.end())
		return NULL;

	return iter->second->GetShader();
}

void ShaderManager::Shader_Begin(const TCHAR * shaderKey)
{
	auto iter = ShaderMap.find(shaderKey);
	if (iter == ShaderMap.end())
		return;

	return iter->second->Shader_Begin();
}

void ShaderManager::Shader_End(const TCHAR * shaderKey)
{
	auto iter = ShaderMap.find(shaderKey);
	if (iter == ShaderMap.end())
		return;

	return iter->second->Shader_End();
}

void ShaderManager::Release()
{
	for (auto iter = ShaderMap.begin();
		iter != ShaderMap.end(); ++iter)
		SAFE_DELETE(iter->second);
	ShaderMap.clear();
}

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
	Release();
}

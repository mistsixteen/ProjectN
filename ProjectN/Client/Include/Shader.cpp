#include "stdafx.h"
#include "Shader.h"


HRESULT Shader::InitShader(const TCHAR * fileName)
{
	if (FAILED(D3DXCreateEffectFromFile(GET_SINGLE(DXFramework)->GetDevice(), 
				fileName, NULL, NULL, D3DXSHADER_DEBUG, NULL,
				&effect, &error)))
	{
		MSGBOX((LPCTSTR)error->GetBufferPointer());
		return E_FAIL;
	}

	return S_OK;
}

void Shader::Shader_Begin()
{
	UINT cnt;
	effect->Begin(&cnt, NULL);
	for (size_t i = 0; i < cnt; ++i)
		effect->BeginPass(i);
}

void Shader::Shader_End()
{
	GET_SINGLE(DXFramework)->GetDevice()->SetVertexShader(NULL);
	GET_SINGLE(DXFramework)->GetDevice()->SetPixelShader(NULL);
	effect->EndPass();
	effect->End();
}

void Shader::Release()
{
	SAFE_DELETE(effect);
	SAFE_DELETE(error);
}

Shader::Shader()
{
}


Shader::~Shader()
{
	Release();
}

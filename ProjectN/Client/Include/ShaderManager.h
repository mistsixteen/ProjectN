#pragma once
#include "Define.h"
#include "Shader.h"

class Shader;
class ShaderManager
{
	DECLARE_SINGLETON(ShaderManager);
private:
	map<const TCHAR*, Shader*>		ShaderMap;
public:
	HRESULT AddShader(const TCHAR* shaderKey, const TCHAR* fileName);
	LPD3DXEFFECT GetShader(const TCHAR* shaderKey);
	void Shader_Begin(const TCHAR* shaderKey);
	void Shader_End(const TCHAR* shaderKey);
	void Release();
private:
	ShaderManager();
public:
	~ShaderManager();
};


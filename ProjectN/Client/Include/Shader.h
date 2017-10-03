#pragma once
#include "Define.h"
#include "DXFramework.h"

class Shader
{
private:
	LPD3DXEFFECT	effect;
	LPD3DXBUFFER	error;
public:
	HRESULT InitShader(const TCHAR* fileName);
	LPD3DXEFFECT GetShader() { return effect; }
public:
	void Shader_Begin();
	void Shader_End();
	void Release();
public:
	Shader();
	~Shader();
};


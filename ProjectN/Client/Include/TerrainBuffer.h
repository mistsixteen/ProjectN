#pragma once
#include "Buffer.h"
class TerrainBuffer :
	public Buffer
{
public:
	virtual HRESULT Initialize();
public:
	TerrainBuffer();
	~TerrainBuffer();
};


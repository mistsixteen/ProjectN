#pragma once
#include "Buffer.h"

class RectangleBuffer :
	public Buffer
{
public:
	virtual HRESULT Initialize();
public:
	RectangleBuffer();
	~RectangleBuffer();
};


#pragma once
#include "Buffer.h"
class BoxBuffer :
	public Buffer
{
public:
	virtual HRESULT Initialize();
public:
	BoxBuffer();
	~BoxBuffer();
};


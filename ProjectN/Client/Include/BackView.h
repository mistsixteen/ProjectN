#pragma once
#include "Camera.h"

class BackView :
	public Camera
{
public:
	virtual void Initialize();
	virtual void Progress();
public:
	BackView();
	~BackView();
};


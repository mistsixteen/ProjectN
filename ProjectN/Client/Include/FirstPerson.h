#pragma once
#include "Camera.h"

class FirstPerson :
	public Camera
{
public:
	virtual void Initialize();
	virtual void Progress(); 
public:
	FirstPerson();
	~FirstPerson();
};


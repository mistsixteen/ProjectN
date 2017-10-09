#pragma once
#include "Camera.h"

class FirstPerson :
	public Camera
{
private:
	INFO			targetInfo;
public:
	virtual void Initialize();
	virtual void Progress(); 
public:
	FirstPerson();
	~FirstPerson();
};


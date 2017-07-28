#pragma once

const int WINSIZEX = 1280;
const int WINSIZEY = 720;

//const int WINSIZEX = 800;
//const int WINSIZEY = 600;

/**
* ������ â ��� 
**/
enum WINTYPE
{
	WINTYPE_FULL,
	WINTYPE_WIN,
};

/**
* ȭ�� Ÿ��
**/
enum SCENE_TYPE
{
	SCENE_TYPE_LOGO,
	SCENE_TYPE_TRAINING,
	SCENE_TYPE_END,
};

/**
* �������� ������Ʈ Ÿ��
**/
enum OBJECT_TYPE
{
	OBJECT_TYPE_ALLY,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_TARGET,
	OBJECT_TYPE_BLOCK,
	OBJECT_TYPE_END,
};
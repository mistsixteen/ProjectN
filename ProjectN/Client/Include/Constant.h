#pragma once

const int WINSIZEX	= 1280;
const int WINSIZEY	= 800;

//const int WINSIZEX = 800;
//const int WINSIZEY = 600;

/*
* VTXCNT ��쿣 ��ȹ�� ���� + 1 �� ���� �ʿ�
*/
const int VTXCNTX = 65;
const int VTXCNTZ = 65;
const float VTXGAP = 1.f;


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
enum SCENE
{
	SCENE_LOGO,
	SCENE_TRAINING,
	SCENE_END,
};

/**
* �������� ������Ʈ Ÿ��
**/
enum OBJECT
{
	OBJECT_ALLY,
	OBJECT_BULLET,
	OBJECT_TARGET,
	OBJECT_BLOCK,
	OBJECT_END,
};

/**
* ī�޶� Ÿ��
*/
enum CAMERA
{
	CAMERA_FIRST_PERSON,
	CAMERA_BACKVIEW,
	CAMERA_END,
};

/**
* ī�޶� Ȥ�� ������Ʈ ȸ�� Ÿ��
*/
enum ANGLE
{
	ANGLE_X,
	ANGLE_Y,
	ANGLE_Z,
	ANGLE_END,
};


/**
* �ؽ��� Ÿ��
*/
enum TEXTYPE
{
	TEXTYPE_GENERAL,
	TEXTYPE_CUBE,
};

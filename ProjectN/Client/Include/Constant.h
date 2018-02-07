#pragma once

const int WINSIZEX	= 1280;
const int WINSIZEY	= 800;

//const int WINSIZEX = 800;
//const int WINSIZEY = 600;

/*
* VTXCNT 경우엔 계획된 정점 + 1 씩 선언 필요
*/
const int VTXCNTX = 65;
const int VTXCNTZ = 65;
const float VTXGAP = 1.f;


/**
* 윈도우 창 모드 
**/
enum WINTYPE
{
	WINTYPE_FULL,
	WINTYPE_WIN,
};

/**
* 화면 타입
**/
enum SCENE
{
	SCENE_LOGO,
	SCENE_TRAINING,
	SCENE_END,
};

/**
* 스테이지 오브젝트 타입
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
* 카메라 타입
*/
enum CAMERA
{
	CAMERA_FIRST_PERSON,
	CAMERA_BACKVIEW,
	CAMERA_END,
};

/**
* 카메라 혹은 오브젝트 회전 타입
*/
enum ANGLE
{
	ANGLE_X,
	ANGLE_Y,
	ANGLE_Z,
	ANGLE_END,
};


/**
* 텍스쳐 타입
*/
enum TEXTYPE
{
	TEXTYPE_GENERAL,
	TEXTYPE_CUBE,
};

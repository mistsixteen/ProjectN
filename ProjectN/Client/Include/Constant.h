#pragma once

const int WINSIZEX = 1280;
const int WINSIZEY = 720;

//const int WINSIZEX = 800;
//const int WINSIZEY = 600;

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
enum SCENE_TYPE
{
	SCENE_TYPE_LOGO,
	SCENE_TYPE_TRAINING,
	SCENE_TYPE_END,
};

/**
* 스테이지 오브젝트 타입
**/
enum OBJECT_TYPE
{
	OBJECT_TYPE_ALLY,
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_TARGET,
	OBJECT_TYPE_BLOCK,
	OBJECT_TYPE_END,
};
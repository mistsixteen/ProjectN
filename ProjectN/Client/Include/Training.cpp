#include "stdafx.h"
#include "Training.h"

Training::Training()
{

}

Training::~Training()
{
	Release();
}

HRESULT Training::Initialize()
{
	// 카메라 초기화
	GET_SINGLE(CameraManager)->InitCamera(CAMERA_FIRST_PERSON);

	// 메쉬 추가
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Terrain", L"Ground")))
	{
		MSGBOX(L"Training : Terrain 메쉬 추가 실패");
		return E_FAIL;
	}
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Box", L"Box")))
	{
		MSGBOX(L"Training : Box 메쉬 추가 실패");
		return E_FAIL;
	}
	
	// 오브젝트 추가
	INFO info;
	ZeroMemory(&info, sizeof(INFO));
	GET_SINGLE(ObjectManager)->InsertObject(L"Ground", Factory::CreateInstance(L"Ground", L"Ground", info));
	GET_SINGLE(ObjectManager)->InsertObject(L"Box", Factory::CreateInstance(L"Player", L"Player", info));
	
	return S_OK;
}

void Training::Progress()
{
	GET_SINGLE(CameraManager)->Progress();
	GET_SINGLE(ObjectManager)->Progress();
}

void Training::Render()
{
	GET_SINGLE(ObjectManager)->Render();
}

void Training::Release()
{
	GET_SINGLE(MeshManager)->Release();
	GET_SINGLE(ObjectManager)->Release();
}


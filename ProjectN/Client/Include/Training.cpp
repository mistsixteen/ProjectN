#include "stdafx.h"
#include "Training.h"

Terrain* t;
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

	// 터레인 메쉬 및 오브젝트 추가
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Terrain", MESH_TERRAIN)))
	{
		MSGBOX(L"Training : Terrain 메쉬 불러오기 실패");
		return E_FAIL;
	}
	//GET_SINGLE(ObjectManager)->InsertObject(L"Terrain", Factory::CreateInstance(L"Terrain", L"Terrain"));
	t = new Terrain(L"");
	t->Initialize();

	return S_OK;
}

void Training::Progress()
{
	GET_SINGLE(CameraManager)->Progress();
	GET_SINGLE(ObjectManager)->Progress();
	t->Progress();
}

void Training::Render()
{
	GET_SINGLE(ObjectManager)->Render();
	t->Render();
}

void Training::Release()
{
	GET_SINGLE(ObjectManager)->Release();
	SAFE_DELETE(t);
}


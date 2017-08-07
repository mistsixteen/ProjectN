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
	// ī�޶� �ʱ�ȭ
	GET_SINGLE(CameraManager)->InitCamera(CAMERA_FIRST_PERSON);

	// �޽� �߰�
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Terrain", L"Ground")))
	{
		MSGBOX(L"Training : Terrain �޽� �߰� ����");
		return E_FAIL;
	}
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Box", L"Box")))
	{
		MSGBOX(L"Training : Box �޽� �߰� ����");
		return E_FAIL;
	}
	
	// ������Ʈ �߰�
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


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
	// �޽� �߰�
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Terrain", L"Terrain")))
	{
		MSGBOX(L"Training : Terrain �޽� �߰� ����");
		return E_FAIL;
	}
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Player", L"Player")))
	{
		MSGBOX(L"Training : Box �޽� �߰� ����");
		return E_FAIL;
	}
	
	// ������Ʈ �߰�
	INFO info;
	ZeroMemory(&info, sizeof(INFO));
	GET_SINGLE(ObjectManager)->InsertObject(L"Terrain", Factory::CreateInstance(L"Terrain", L"Terrain", info));

	ZeroMemory(&info, sizeof(INFO));
	info.position = D3DXVECTOR3((VTXCNTX - 1) * 0.5f * VTXGAP, 5.f, (VTXCNTZ - 1) * 0.5f * VTXGAP);
	info.look = D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVec3Normalize(&info.look, &info.look);
	GET_SINGLE(ObjectManager)->InsertObject(L"Player", Factory::CreateInstance(L"Player", L"Player", info));

	// ī�޶� �ʱ�ȭ
	GET_SINGLE(CameraManager)->InitCamera(CAMERA_FIRST_PERSON);

	return S_OK;
}

void Training::Progress()
{
	GET_SINGLE(ObjectManager)->Progress();
	GET_SINGLE(CameraManager)->Progress();
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


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
	// 오브젝트 추가
	INFO info;
	ZeroMemory(&info, sizeof(INFO));
	GET_SINGLE(ObjectManager)->AddObject(L"Terrain", Factory<Terrain>::CreateInstance(L"Terrain", info));

	ZeroMemory(&info, sizeof(INFO));
	info.position = D3DXVECTOR3((VTXCNTX - 1) * 0.5f * VTXGAP, 0.f, (VTXCNTZ - 1) * 0.5f * VTXGAP);
	GET_SINGLE(ObjectManager)->AddObject(L"Background", Factory<Background>::CreateInstance(L"Background", info));

	ZeroMemory(&info, sizeof(INFO));
	info.position = D3DXVECTOR3((VTXCNTX - 1) * 0.5f * VTXGAP, 1.f, (VTXCNTZ - 1) * 0.5f * VTXGAP);
	info.look = D3DXVECTOR3(0.f, 0.f, 1.f);
	GET_SINGLE(ObjectManager)->AddObject(L"Player", Factory<Player>::CreateInstance(L"Player", info));

	// 카메라 초기화
	GET_SINGLE(CameraManager)->InitCamera(CAMERA_BACKVIEW);

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
	GET_SINGLE(BufferManager)->Release();
	GET_SINGLE(ObjectManager)->Release();
}


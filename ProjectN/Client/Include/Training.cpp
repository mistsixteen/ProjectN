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
	// �ͷ���, ���, �÷��̾�, Ÿ�� ������ ����, �޽� �� ������Ʈ �ʱ�ȭ
	// ���� �ʱ�ȭ 
	if (FAILED(GET_SINGLE(BufferManager)->AddBuffer(L"Terrain", L"Terrain")))
	{
		MSGBOX(L"Terrain ���� �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(BufferManager)->AddBuffer(L"Background", L"Box")))
	{
		MSGBOX(L"��׶��� box ���� �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(BufferManager)->AddBuffer(L"Player", L"Box")))
	{
		MSGBOX(L"�÷��̾� box ���� �߰� ����");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(BufferManager)->AddBuffer(L"Target", L"Box")))
	{
		MSGBOX(L"ǥ�� box ���� �߰� ����");
		return E_FAIL;
	}
	if (FAILED(GET_SINGLE(BufferManager)->AddBuffer(L"UI", L"Rectangle")))
	{
		MSGBOX(L"UI ���� �߰� ����");
		return E_FAIL;
	}

	// �޽� �ʱ�ȭ
	if (FAILED(GET_SINGLE(MeshManager)->AddMesh(L"Bullet", L"Sphere")))
	{
		MSGBOX(L"��ź sphere �߰� ����");
		return E_FAIL;
	}

	// ������Ʈ �߰�
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

	ZeroMemory(&info, sizeof(INFO));
	GET_SINGLE(ObjectManager)->AddObject(L"Target", Factory<Target>::CreateInstance(L"Player", info));
	GET_SINGLE(ObjectManager)->AddObject(L"Target", Factory<Target>::CreateInstance(L"Player", info));


	ZeroMemory(&info, sizeof(INFO));
	//info.position = D3DXVECTOR3((VTXCNTX - 1) * 0.5f * VTXGAP, 0.5f, (VTXCNTZ - 1) * 0.5f * VTXGAP);
	//GET_SINGLE(ObjectManager)->AddObject(L"Bullet", Factory<Bullet>::CreateInstance(L"Bullet", info));

	// UI �߰�
	GET_SINGLE(UIManager)->AddUI(L"Aim", Factory<Aim>::CreateInstance(L"Aim"));

	// ī�޶� �ʱ�ȭ
	GET_SINGLE(CameraManager)->InitCamera(CAMERA_BACKVIEW);

	return S_OK;
}

void Training::Progress()
{
	GET_SINGLE(CameraManager)->Progress();
	GET_SINGLE(ObjectManager)->Progress();
	GET_SINGLE(UIManager)->Progress();
}

void Training::Render()
{
	GET_SINGLE(ObjectManager)->Render();
	GET_SINGLE(UIManager)->Render();
}

void Training::Release()
{
	GET_SINGLE(MeshManager)->Release();
	GET_SINGLE(BufferManager)->Release();
	GET_SINGLE(ObjectManager)->Release();
	GET_SINGLE(UIManager)->Release();
}


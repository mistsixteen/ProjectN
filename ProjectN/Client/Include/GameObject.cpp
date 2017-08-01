#include "stdafx.h"
#include "GameObject.h"


HRESULT GameObject::Initialize(void)
{

	if (FAILED(GET_SINGLE(DXFramework)->GetDevice()->
		CreateVertexBuffer(vertexSize * vertexCount,
			0, vertexFVF, D3DPOOL_MANAGED,
			&vertexBuffer, NULL)))
	{
		MSGBOX(L"GameObject initialization : 버텍스 버퍼 생성 실패");
		return E_FAIL;
	}

	if (FAILED(GET_SINGLE(DXFramework)->GetDevice()->
		CreateIndexBuffer(indexSize * triangleCount,
			0, indexFormat, D3DPOOL_MANAGED,
			&indexBuffer, NULL)))
	{
		MSGBOX(L"GameObject initialization : 인덱스 버퍼 생성 실패");
		return E_FAIL;
	}

	return S_OK;
}

void GameObject::Render(void)
{
	GET_SINGLE(DXFramework)->GetDevice()->SetStreamSource(0, vertexBuffer, 0, vertexSize);
	GET_SINGLE(DXFramework)->GetDevice()->SetFVF(vertexFVF);
	GET_SINGLE(DXFramework)->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertexCount, 0, triangleCount);
}

void GameObject::Release(void)
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

GameObject::GameObject()
	:key(NULL)
{
}

GameObject::GameObject(const TCHAR * _key)
	:key(_key)
{
}


GameObject::~GameObject()
{
	Release();
}

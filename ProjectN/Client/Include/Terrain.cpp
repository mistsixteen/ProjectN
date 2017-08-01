#include "stdafx.h"
#include "Terrain.h"

HRESULT Terrain::Initialize()
{
	// �ͷ��� ���� ���� üũ
	if (vertexCountX <= 0 || vertexCountZ <= 0 || vertexGap <= 0)
	{
		TCHAR msg[128] = L"";
		wsprintf(msg, L"Terrain initialization : �߸��� ���ؽ� ���� �ֽ��ϴ�. (x: %d / z: %d / gap: %d)", vertexCountX, vertexCountZ, vertexGap);
		MSGBOX(msg);
		return E_FAIL;
	}

	// �ͷ��� ���� �ʱ�ȭ
	vertexCount = vertexCountX * vertexCountZ;
	vertexSize = sizeof(VTXTEX);
	vertexFVF = VTXFVF_TEX;

	indexFormat = D3DFMT_INDEX32;
	indexSize = sizeof(INDEX);
	triangleCount = (vertexCountX - 1) * (vertexCountZ - 1) * 2;

	// �ͷ��� ���ؽ� ����
	VTXTEX* verticeTex = NULL;
	vertexBuffer->Lock(0, 0, (void**)&verticeTex, 0);

	for (int z = 0; z < vertexCountZ; ++z)
	{
		for (int x = 0; x < vertexCountX; ++x)
		{
			int index = (z * vertexCountX) + x;
			verticeTex[index].vPos = D3DXVECTOR3(float(x * vertexGap), 0.f, float(z * vertexGap));
			verticeTex[index].vTexture = D3DXVECTOR2(float(x) / (vertexCountX - 1), float(z) / (vertexCountZ - 1));
		}
	}

	vertexBuffer->Unlock();

	// �ͷ��� �ε��� ����
	INDEX* indices = NULL;
	int	triCount = 0;
	D3DXVECTOR3 dest, src, result; 
	indexBuffer->Lock(0, 0, (void**)&indices, 0);

	for (int z = 0; z < vertexCountZ - 1; ++z)
	{
		for (int x = 0; x < vertexCountX - 1; ++x)
		{
			int iIndex = (z * vertexCountZ) + x;

			//���� ���
			indices[triCount]._1 = vertexCountX + iIndex;
			indices[triCount]._2 = vertexCountX + 1 + iIndex;
			indices[triCount]._3 = iIndex + 1;

			dest = verticeTex[indices[triCount]._2].vPos
				- verticeTex[indices[triCount]._1].vPos;

			src = verticeTex[indices[triCount]._3].vPos
				- verticeTex[indices[triCount]._1].vPos;
			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);
			verticeTex[indices[triCount]._1].vNormal += result;
			verticeTex[indices[triCount]._2].vNormal += result;
			verticeTex[indices[triCount]._3].vNormal += result;
			++triCount;

			//���� �ϴ�
			indices[triCount]._1 = vertexCountX + iIndex;
			indices[triCount]._2 = iIndex + 1;
			indices[triCount]._3 = iIndex;

			dest = verticeTex[indices[triCount]._2].vPos
				- verticeTex[indices[triCount]._1].vPos;
			src = verticeTex[indices[triCount]._3].vPos
				- verticeTex[indices[triCount]._1].vPos;
			D3DXVec3Cross(&result, &dest, &src);
			D3DXVec3Normalize(&result, &result);
			verticeTex[indices[triCount]._1].vNormal += result;
			verticeTex[indices[triCount]._2].vNormal += result;
			verticeTex[indices[triCount]._3].vNormal += result;

			++triCount;
		}
	}

	for (int i = 0; i < vertexCount; ++i)
		D3DXVec3Normalize(&verticeTex[i].vNormal, &verticeTex[i].vNormal);

	indexBuffer->Unlock();

	// ���ؽ� �� �ε��� ���� �ʱ�ȭ
	if (FAILED(GameObject::Initialize()))
		return E_FAIL;

	// ��Ʈ���� �ʱ�ȭ
	D3DXMatrixTranslation(&information.world, 
						information.position.x, 
						information.position.y, 
						information.position.z);

	return S_OK;
}

void Terrain::Progress()
{
}

void Terrain::Render()
{
	GameObject::Render();
}

void Terrain::Release()
{
}

Terrain::Terrain()
{
}

Terrain::Terrain(const TCHAR* _key, const int _vertexCountX, const int _vertexCountZ, const int _vertexGap)
	:GameObject(_key),
	vertexCountX(_vertexCountX),
	vertexCountZ(_vertexCountZ),
	vertexGap(_vertexGap)
{
	ZeroMemory(&information, sizeof(information));
	D3DXMatrixIdentity(&information.world);
}

Terrain::~Terrain()
{
	Release();
}

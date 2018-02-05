#include "stdafx.h"
#include "Target.h"

// 일정 시간 후 부활시키는 임시 변수
bool isDead = true;
chrono::system_clock::time_point start;

HRESULT Target::Initialize(void)
{
	information.min = *(GET_SINGLE(BufferManager)->GetMin(L"Target"));
	information.max = *(GET_SINGLE(BufferManager)->GetMax(L"Target"));

	// 셰이더 초기화
	GET_SINGLE(ShaderManager)->AddShader(L"Target", L"./Resource/Shader/Target.hpp");

	// 난수 생성 -> 크기 및 위치 초기화
	unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
	mt19937_64 ranNum(seed);

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, ranNum() % 11 * 0.1f + 1.f, ranNum() % 11 * 0.1f + 1.f, ranNum() % 11 * 0.1f + 1.f);
	information.position = { ranNum() % (VTXCNTX - 1) * VTXGAP,
							0,
							ranNum() % (VTXCNTZ - 1) * VTXGAP};

	// 경계 설정
	if (information.position.x > (VTXCNTX - 1) * VTXGAP - information.max.x) {
		information.position.x = (VTXCNTX - 1) * VTXGAP - information.max.x;
	}
	if (information.position.x < -information.min.x) {
		information.position.x = -information.min.x;
	}

	if (information.position.y >(VTXCNTX - 1) * VTXGAP - information.max.y) {
		information.position.y = (VTXCNTX - 1) * VTXGAP - information.max.y;
	}
	if (information.position.y < -information.min.y) {
		information.position.y = -information.min.y;
	}

	if (information.position.z >(VTXCNTZ - 1) * VTXGAP - information.max.z) {
		information.position.z = (VTXCNTZ - 1) * VTXGAP - information.max.z;
	}
	if (information.position.z < -information.min.z) {
		information.position.z = -information.min.z;
	}

	// 체력 초기화
	information.hp = 1;

	return S_OK;
}

void Target::Progress(void)
{
}

void Target::Render(void)
{
	if (information.hp > 0) {
		GameObject::Render();

		// 셰이더 변수 설정
		LPD3DXEFFECT effect = GET_SINGLE(ShaderManager)->GetShader(L"Target");

		D3DXMATRIX matView, matProj;
		GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

		effect->SetMatrix("gWorldMatrix", &world);
		effect->SetMatrix("gViewMatrix", &matView);
		effect->SetMatrix("gProjectionMatrix", &matProj);

		D3DXVECTOR4 color;
		switch (information.hp) {
		case 1:
			color = D3DXVECTOR4(1.f, 0.f, 0.f, 1.f);
			break;
		case 2:
			color = D3DXVECTOR4(1.f, 1.f, 0.f, 1.f);
			break;
		case 3:
			color = D3DXVECTOR4(0.f, 1.f, 0.f, 1.f);
			break;
		default:
			color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		}
		effect->SetVector("gColor", &color);

		// 메쉬 렌더링
		GET_SINGLE(ShaderManager)->Shader_Begin(L"Target");
		GET_SINGLE(BufferManager)->Render(L"Target");
		GET_SINGLE(ShaderManager)->Shader_End(L"Target");


		//for_test
		//render box of obb -> position based, 1^3 box
		ID3DXLine *templine;
		D3DXCreateLine(GET_SINGLE(DXFramework)->GetDevice(), &templine);
		templine->SetWidth(2.0f);
		D3DXVECTOR3 obbAxis[3] = {
			D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 1.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 1.0f)
		};
		float length[3] = {0.5f, 0.5f, 0.5f};
		D3DXVECTOR3 vList[8];
		int target = 0;
		for(int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					float a = ((float)i - 0.5f) * 1.0f;
					float aa = ((float)j - 0.5f) * 1.0f;
					float aaa = ((float)k - 0.5f) * 1.0f;

					//
					D3DXVECTOR3 change = ((float)i - 0.5f) * obbAxis[0] + ((float)j - 0.5f) * obbAxis[1] + ((float)k - 0.5f) * obbAxis[2];
					
						
					vList[target] = information.position + change;


					target = target + 1;
				}
			}
		}
		D3DXMATRIX temp = matView * matProj;
		templine->Begin();
		templine->DrawTransform(vList, 8, &temp, D3DCOLOR_XRGB(255, 255, 0));
		//templine->Draw()
		templine->End();

		templine->Release();
	}
}

void Target::Release(void)
{
}

void Target::Oncolide(void)
{
	information.hp = 3;
}

Target::Target()
{
}

Target::Target(const TCHAR * _key, INFO _info)
	:GameObject(_key, _info)
{
}


Target::~Target()
{
	Release();
}

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
							ranNum() % (VTXCNTX - 1) * VTXGAP,
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
	information.hp = 0;

	return S_OK;
}

void Target::Progress(void)
{
}

void Target::Render(void)
{
	if (information.hp <= 0) {
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

		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		GET_SINGLE(BufferManager)->Render(L"Target");
		GET_SINGLE(DXFramework)->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);

		GET_SINGLE(ShaderManager)->Shader_End(L"Target");
	}
}

void Target::Release(void)
{
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

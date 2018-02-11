#include "stdafx.h"
#include "Player.h"


#include "DXFramework.h"

HRESULT Player::Initialize(void)
{
	information.min = *(GET_SINGLE(BufferManager)->GetMin(L"Player"));
	information.max = *(GET_SINGLE(BufferManager)->GetMax(L"Player"));

	// 셰이더 초기화
	GET_SINGLE(ShaderManager)->AddShader(L"Player", L"./Resource/Shader/Player.hpp");

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);

	// 플레이어 속도 초기화
	information.speed = 10.f;

	// 비행 상태 초기화
	flying = true;
	
	// 낙하 속도 초기화
	fallingSpeed = 2.0f;

	// 임시 : boost 관련 변수들 초기화

	boostspeed = 0.0f;
	bospeedmax = 2.4f;
	bospeedincrese = 0.1f;
	bospeeddecrese = 0.3f;

	burstspeed = 0.0f;
	buspeedmax = 20.0f;
	buspeeddecrese = 3.0f;

	temp_firebulletdelay = 0.0f;

	// 매트릭스 초기화
	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 1.f, 1.f, 1.f);

	return S_OK;
}

void Player::Progress(void)
{
	prevPos = information.position;
	if (GET_SINGLE(DXInput)->PushRight())
		KeyCheck();
	SetPosition();
}

void Player::SetPosition()
{
	float height = GET_SINGLE(ObjectManager)->GetTerrainHeight(information);
	// 터레인에 벗어났을 경우
	if (height == -1.f)
	{
		// 부드러운 이동 불가, 수정 필요
		information.position = prevPos;
	}
	else
	{
		// 터레인 통과 불가
		if (information.position.y < height)
			information.position.y = height;
	}
}

void Player::KeyCheck()
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 move = D3DXVECTOR3(0.f, 0.f, 0.f);

	// 직진 이동
	if (PUSH_KEY(DIK_W))
	{
		move = information.look;

		move.y = 0.0f;
		
		D3DXVec3Normalize(&move, &move);
		information.position += move * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 후진 이동
	if (PUSH_KEY(DIK_S))
	{
		move = information.look;

		move.y = 0.0f;

		D3DXVec3Normalize(&move, &move);
		information.position -= move * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 좌측 이동
	if (PUSH_KEY(DIK_A))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position -= vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}
	// 우측 이동
	if (PUSH_KEY(DIK_D))
	{
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &up, &information.look);
		D3DXVec3Normalize(&vRight, &vRight);

		information.position += vRight * GET_SINGLE(TimeManager)->GetTime() * information.speed;
	}

	//falling
	D3DXVECTOR3 failing;
	failing.x = 0.0f;
	failing.y = fallingSpeed * -1.0f;
	failing.z = 0.0f;


	information.position += failing * GET_SINGLE(TimeManager)->GetTime() * information.speed;

	//fallingSpeed

	if (PUSH_KEY(DIK_1)) //boost
	{
	
		boostspeed = boostspeed + bospeedincrese;

		if (boostspeed > bospeedmax)
			boostspeed = bospeedmax;
	}
	else
	{
		boostspeed = boostspeed - bospeeddecrese;

		if (boostspeed < 0.0f)
			boostspeed = 0.0f;
	}

	D3DXVECTOR3 boost;
	boost.x = 0.0f;
	boost.y = boostspeed;
	boost.z= 0.0f;

	information.position += boost * GET_SINGLE(TimeManager)->GetTime() * information.speed;

	/*

	burstspeed = 0.0f;
	buspeedmax = 10.0f;
	buspeeddecrese = 3.0f;
	*/

	if (PUSH_KEY(DIK_2)) //burst
	{
		if (burstspeed == 0)
		{
			burstspeed = buspeedmax;
		}
		else
		{
			burstspeed = burstspeed - buspeeddecrese;
		}
	}
	else
	{
		burstspeed = burstspeed - buspeeddecrese;
	}

	if (burstspeed < 0.0f)
		burstspeed = 0.0f;


	D3DXVECTOR3 burst;
	burst.x = 0.0f;
	burst.y = burstspeed;
	burst.z = 0.0f;

	information.position += burst * GET_SINGLE(TimeManager)->GetTime() * information.speed;

	INFO info;
	ZeroMemory(&info, sizeof(INFO));

	if (PUSH_KEY(DIK_3)) //임시_firebullet
	{
		if(temp_firebulletdelay <= 0.0f)
		{
			GET_SINGLE(ObjectManager)->AddObject(L"Bullet", Factory<Bullet>::CreateInstance(L"Bullet", this->information));
			temp_firebulletdelay = 30.0f;
		}
	}

	if (temp_firebulletdelay > 0.0f) temp_firebulletdelay = temp_firebulletdelay - 0.1f;
}

void Player::Render(void)
{

	GameObject::Render();

	// 셰이더 변수 설정
	LPD3DXEFFECT effect = GET_SINGLE(ShaderManager)->GetShader(L"Player");
	effect->SetMatrix("gWorldMatrix", &world);

	D3DXMATRIX matView, matProj;
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GET_SINGLE(DXFramework)->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

	effect->SetMatrix("gWorldMatrix", &world);
	effect->SetMatrix("gViewMatrix", &matView);
	effect->SetMatrix("gProjectionMatrix", &matProj);

	// 메쉬 렌더링
	GET_SINGLE(ShaderManager)->Shader_Begin(L"Player");
	GET_SINGLE(BufferManager)->Render(L"Player");
	GET_SINGLE(ShaderManager)->Shader_End(L"Player");
}

void Player::Release(void)
{
}

Player::Player()
{
}

Player::Player(const TCHAR * _key, INFO _info)
	:GameObject(_key, _info)
{
	memset(&prevPos, NULL, sizeof(D3DXVECTOR3));
}

void Player::Oncolide(void)
{

}


Player::~Player()
{
	Release();
}

/*TCHAR strTmp[128] = L"";
wsprintf(strTmp, L"Look : %d %d %d",
(int)information.look.x, (int)information.look.y, (int)information.look.z);
GET_SINGLE(DXFramework)->Drawtext(strTmp, 0, 0);*/
//GET_SINGLE(MeshManager)->Mesh_Render(L"Player");


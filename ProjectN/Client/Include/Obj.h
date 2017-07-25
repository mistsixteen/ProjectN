#pragma once
class CObj
{
protected:
	D3DXVECTOR3		position;
	D3DXVECTOR3		direction;
	D3DXVECTOR3		look;
	D3DXMATRIX		world;
public:
	virtual HRESULT Initialize(void) PURE;
	virtual	void	Progress(void) PURE;
	virtual	void	Render(void) PURE;
	virtual	void	Release(void);
public:
	CObj();
	virtual ~CObj();
};


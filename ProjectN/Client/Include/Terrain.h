#pragma once
#include "StaticInst.h"
class CTerrain :
	public CStaticInst
{

public:
	virtual HRESULT Initialize();
	virtual void Progress();
	virtual void Render();
	virtual void Release();
public:
	CTerrain(const int _iVtxCntX, const int _iVtxCntZ, const int _iVtxGap);
	~CTerrain();
};


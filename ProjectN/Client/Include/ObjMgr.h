#pragma once
#include "Define.h"
#include "HashTable.h"

#include "Obj.h"

class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr);
private:
	CHashTable<CObj*>	hashTable;
public:
	void InsertObject(const TCHAR* objectKey, CObj* object);
	void Progress(void);
	void Render(void);
	void Release(void);
private:
	CObjMgr();
public:
	~CObjMgr();
};


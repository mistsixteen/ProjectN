#pragma once

#define PUSH_KEY(key) (GetKeyBoardState(key)&0x80) ? true : false
#define UP_KEY(key) (GetKeyBoardState(key)&0x80) ? false :true 

// �̱��� ���� Ŭ���� ���� ��ó����
#define DECLARE_SINGLETON(type) public:		\
	static type** GetInstance()				\
	{										\
		static type* pInstance = new type;	\
		if(pInstance == NULL)				\
			pInstance = new type;			\
		return &pInstance;					\
	}										\
	static void DestroyInstance()			\
	{										\
		type** ppInstance = GetInstance();	\
		if(*ppInstance != NULL)				\
		{									\
			delete *ppInstance;				\
			*ppInstance = NULL;				\
		}									\
	}

// �̱��� �Լ� �ҷ����� ��ó����
#define GET_SINGLE(type) (*type::GetInstance())

// ������ ���� ��ó����
#define SAFE_DELETE(pointer) if(pointer) {delete pointer; pointer=NULL;}
#define SAFE_DELETE_ARRAY(pointer) if(pointer){ delete[] pointer; pointer= NULL;}
#define SAFE_DELETE_SINGLE(type) {(*type::GetInstance())->DestroyInstance(); }
#define SAFE_RELEASE(pointer) if(pointer){ pointer->Release(); pointer = NULL; }

// �޽��� �ڽ� ��ó����
#define MSGBOX(str) MessageBox(NULL, str, L"Message", MB_OK)
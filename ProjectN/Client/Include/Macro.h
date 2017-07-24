#pragma once

#define PUSH_KEY(key) (GetKeyBoardState(key)&0x80) ? true : false
#define UP_KEY(key) (GetKeyBoardState(key)&0x80) ? false :true 

// 싱글톤 패턴 클래스 선언 전처리기
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

// 싱글톤 함수 불러오기 전처리기
#define GET_SINGLE(type) (*type::GetInstance())

// 포인터 삭제 전처리기
#define SAFE_DELETE(pointer) if(pointer) {delete pointer; pointer=NULL;}
#define SAFE_DELETE_ARRAY(pointer) if(pointer){ delete[] pointer; pointer= NULL;}
#define SAFE_DELETE_SINGLE(type) {(*type::GetInstance())->DestroyInstance(); }
#define SAFE_RELEASE(pointer) if(pointer){ pointer->Release(); pointer = NULL; }

// 메시지 박스 전처리기
#define MSGBOX(str) MessageBox(NULL, str, L"Message", MB_OK)
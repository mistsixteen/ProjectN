#ifndef _SYSTEMMACRO_H
#define _SYSTEMMACRO_H


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
#define GET_SINGLE(type) (*type::GetInstance())
#define SAFE_DELETE_SINGLE(type) {(*type::GetInstance())->DestroyInstance(); }

#define SAFE_DELETE(pointer) if(pointer) {delete pointer; pointer=NULL;}
#define SAFE_DELETE_ARRAY(pointer) if(pointer){ delete[] pointer; pointer= NULL;}
#define SAFE_RELEASE(pointer) if(pointer){ pointer->Release(); pointer = NULL; }

#define MSGBOX(str) MessageBox(NULL, str, L"Message", MB_OK)
#endif
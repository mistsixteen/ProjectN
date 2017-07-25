#pragma once
#include "Define.h"

template<typename T>
struct tagDataSet
{
	const TCHAR* key;
	T			 Value;

	tagDataSet(const TCHAR* pkey,
		T			 ValueData)
		:key(pkey), Value(ValueData)
	{}
};

template<typename T>
class CHashTable
{
	list<list<tagDataSet<T>*>*>*	m_pChainList;
	int m_iSize;
public:
	list<list<tagDataSet<T>*>*>* operator[] (int iIndex)
	{
		return &m_pChainList[iIndex];
	}
	int Size(void)
	{
		return m_iSize;
	}

	void Insert(const TCHAR* key, T Value)
	{
		tagDataSet<T>* pDataSet = new tagDataSet<T>(key, Value);
		int iIndex = Hashing(key);

		if (m_pChainList[iIndex].empty())
		{
			list<tagDataSet<T>*>*	pInnerList = new list<tagDataSet<T>*>;
			pInnerList->push_back(pDataSet);
			m_pChainList[iIndex].push_back(pInnerList);
		}
		else
		{
			for (list<list<tagDataSet<T>*>*>::iterator iter = m_pChainList[iIndex].begin();
			iter != m_pChainList[iIndex].end(); ++iter)
			{
				if (key == (*iter)->front()->key)
				{
					(*iter)->push_back(pDataSet);
					return;
				}
			}

			list<tagDataSet<T>*>*	pInnerList = new list<tagDataSet<T>*>;
			pInnerList->push_back(pDataSet);
			m_pChainList[iIndex].push_back(pInnerList);

		}


	}
	int Hashing(const TCHAR* key)
	{
		int iLength = lstrlen(key);
		int iResult = 0;
		for (int i = 0; i < iLength; ++i)
		{
			iResult += key[i];
		}
		return iResult % m_iSize;
	}

	void Release(void)
	{
		for (int i = 0; i < m_iSize; ++i)
		{
			for (list<list<tagDataSet<T>*>*>::iterator iter = m_pChainList[i].begin();
			iter != m_pChainList[i].end(); ++iter)
			{
				for (list<tagDataSet<T>*>::iterator iter1 = (*iter)->begin();
				iter1 != (*iter)->end(); ++iter1)
				{
					SAFE_DELETE((*iter1)->Value);
					SAFE_DELETE((*iter1));
				}
				(*iter)->clear();
			}
		}
		SAFE_DELETE_ARRAY(m_pChainList);
	}
	const T Find(const TCHAR* pKey, const int& iCnt = 0)
	{
		int iKey = Hashing(pKey);

		for (list<list<tagDataSet<T>*>*>::iterator iter = m_pChainList[iKey].begin();
		iter != m_pChainList[iKey].end(); ++iter)
		{
			if (!lstrcmp((*iter)->front()->key, pKey))
			{
				list<tagDataSet<T>*>::iterator iter1 = (*iter)->begin();

				for (int i = 0; i < iCnt; ++i)
				{
					++iter1;
				}
				return (*iter1)->Value;
			}
		}

		return NULL;
	}

public:
	CHashTable(const int& ReserveSize)
		:m_pChainList(new list<list<tagDataSet<T>*>*>[ReserveSize]),
		m_iSize(ReserveSize)
	{

	}
	~CHashTable()
	{

	}


};
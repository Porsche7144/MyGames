#pragma once
#include <string>
// �ý��� �޸� -> ���� -> �������޸�
// ���� �޸� -> ����(new) - heap
// �޸𸮰� ������(����)���� �Ҵ� �Ǿ� �־�� �Ѵ�.
template<class T>
class TObjectPool
{
public:
	enum
	{
		POOL_MAX_SIZE = 8192, // 2n��
		POOL_SIZE_MASK = POOL_MAX_SIZE - 1,
	};
	static void PrepareAllocation()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{
			m_mPool[i] = _aligned_malloc(sizeof(T), MEMORY_ALLOCATION_ALIGNMENT);
		}
		//m_TailPos = POOL_MAX_SIZE;
		InterlockedAdd64(&m_TailPos, POOL_MAX_SIZE);
	}
	static void AllFree()
	{
		for (int i = 0; i < POOL_MAX_SIZE; i++)
		{			
			void* val = InterlockedExchangePointer(&m_mPool[i],nullptr);//m_mPool[i] = nullptr;
			if (val != nullptr)
			{
				_aligned_free(m_mPool[i]);
			}			
		}
	}
	// 0 -> ret m_mPool[0];
	// 1 -> ret m_mPool[1];
	// 2~1023
	// 1024 -> ret m_mPool[0];
	// POOL_SIZE_MASK = 0111
	// m_HeadPos= 1, m_mPool[0]  0000 & 0111 = 0
	// m_HeadPos= 2, m_mPool[1]  0001 & 0111 = 1
	// m_HeadPos= 3, m_mPool[2]  0001 & 0111 = 1
	// m_HeadPos= 7, m_mPool[7]  0111 & 0111 = 7

	// m_HeadPos= 8, m_mPool[0]  1000 & 0111 = 0
	// m_HeadPos= 9, m_mPool[1]  1001 & 0111 = 1
	// m_HeadPos= 10, m_mPool[2] 1010 & 0111 = 2

	// 000000 10 0000 0000 = 1024
	// 000000 01 1111 1111 = 1023
	static void* operator new(size_t size) 
	{
		long long pos = InterlockedIncrement64(&m_HeadPos)-1;//m_HeadPos++;
		long long valpos = pos & POOL_SIZE_MASK;
		void* val = InterlockedExchangePointer(&m_mPool[valpos], nullptr);
		if (val != nullptr)
		{
			return val; // ����
		}
		return _aligned_malloc(size, MEMORY_ALLOCATION_ALIGNMENT);
	}
	static void operator delete(void* obj) 
	{
		long long pos = InterlockedIncrement64(&m_TailPos) - 1;//m_HeadPos++;
		long long valpos = pos & POOL_SIZE_MASK;
		void* val = InterlockedExchangePointer(&m_mPool[valpos], obj);
		if (val != nullptr)
		{
			_aligned_free(val);// ���� ������ ����
		}
	}

private:
	static void* volatile m_mPool[POOL_MAX_SIZE];
	// int i=1;  while(i>0) {} ->  while(1) {} -> X
	// volatile i ������ �����ִ�. �׷��� ����ȭ ������!
	static long long volatile m_HeadPos;
	static long long volatile m_TailPos;	
};

template<typename T>
void* volatile TObjectPool<T>::m_mPool[POOL_MAX_SIZE] = {};
template<typename T>
long long volatile TObjectPool<T>::m_HeadPos(0);
template<typename T>
long long volatile TObjectPool<T>::m_TailPos(0);
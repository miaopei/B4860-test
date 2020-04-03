#ifndef SIMEPLE_LOCKS_H_43589435
#define SIMEPLE_LOCKS_H_43589435

#include <string>
#include "uv.h"
#include "SimpleUVExport.h"

// �����������CUVAutoLockʹ�ø�����
class SUV_EXPORT CUVMutex
{
public:
	CUVMutex()
	{
		uv_mutex_init(&mut_);
	}
	~CUVMutex(void)
	{
		uv_mutex_destroy(&mut_);
	}
	void Lock()
	{
		uv_mutex_lock(&mut_);
	}
	void UnLock()
	{
		uv_mutex_unlock(&mut_);
	}
	bool TryLock()
	{
		return uv_mutex_trylock(&mut_) == 0;
	}
private:
	uv_mutex_t mut_;
	friend class CUVCond;
	friend class CUVAutoLock;
private:  // private�У���ֹ���ƺ͸�ֵ
	CUVMutex(const CUVMutex&);             // ��ʵ��
	CUVMutex& operator =(const CUVMutex&); // ��ʵ��
};

class SUV_EXPORT CUVAutoLock
{
public:
	CUVAutoLock(uv_mutex_t* mut): mut_(mut)
	{
		uv_mutex_lock(mut_);
	}
	CUVAutoLock(CUVMutex* mut): mut_(&mut->mut_)
	{
		uv_mutex_lock(mut_);
	}
	~CUVAutoLock(void)
	{
		uv_mutex_unlock(mut_);
	}
private:
	uv_mutex_t* mut_;
private:                                   // private�У���ֹ���ƺ͸�ֵ
	CUVAutoLock(const CUVAutoLock&);       // ��ʵ��
	CUVAutoLock& operator =(const CUVAutoLock&);  // ��ʵ��
};

//��������
class SUV_EXPORT CUVCond
{
public:
	CUVCond()
	{
		uv_cond_init(&cond_);
	}
	~CUVCond(void)
	{
		uv_cond_destroy(&cond_);
	}
	void Signal()
	{
		uv_cond_signal(&cond_);
	}
	void BroadCast()
	{
		uv_cond_broadcast(&cond_);
	}
	void Wait(CUVMutex* mutex)
	{
		uv_cond_wait(&cond_, &mutex->mut_);
	}
	void Wait(uv_mutex_t* mutex)
	{
		uv_cond_wait(&cond_, mutex);
	}
	int Wait(CUVMutex* mutex, uint64_t timeout)
	{
		return uv_cond_timedwait(&cond_, &mutex->mut_, timeout);
	}
	int Wait(uv_mutex_t* mutex, uint64_t timeout)
	{
		return uv_cond_timedwait(&cond_, mutex, timeout);
	}
private:
	uv_cond_t cond_;
private:                                   // private�У���ֹ���ƺ͸�ֵ
	CUVCond(const CUVCond&);               // ��ʵ��
	CUVCond& operator =(const CUVCond&);   // ��ʵ��
};

// �ź���
class SUV_EXPORT CUVSem
{
public:
	explicit CUVSem(int initValue = 0)
	{
		uv_sem_init(&sem_, initValue);
	}
	~CUVSem(void)
	{
		uv_sem_destroy(&sem_);
	}
	void Post()
	{
		uv_sem_post(&sem_);
	}
	void Wait()
	{
		uv_sem_wait(&sem_);
	}
	bool TryWait()
	{
		return uv_sem_trywait(&sem_) == 0;
	}
private:
	uv_sem_t sem_;
private:                               // private�У���ֹ���ƺ͸�ֵ
	CUVSem(const CUVSem&);             // ��ʵ��
	CUVSem& operator =(const CUVSem&); // ��ʵ��
};

// ��д��
class SUV_EXPORT CUVRWLock
{
public:
	CUVRWLock()
	{
		uv_rwlock_init(&rwlock_);
	}
	~CUVRWLock(void)
	{
		uv_rwlock_destroy(&rwlock_);
	}
	void ReadLock()
	{
		uv_rwlock_rdlock(&rwlock_);
	}
	void ReadUnLock()
	{
		uv_rwlock_rdunlock(&rwlock_);
	}
	bool ReadTryLock()
	{
		return uv_rwlock_tryrdlock(&rwlock_) == 0;
	}
	void WriteLock()
	{
		uv_rwlock_wrlock(&rwlock_);
	}
	void WriteUnLock()
	{
		uv_rwlock_wrunlock(&rwlock_);
	}
	bool WriteTryLock()
	{
		return uv_rwlock_trywrlock(&rwlock_) == 0;
	}
private:
	uv_rwlock_t rwlock_;
private://private�У���ֹ���ƺ͸�ֵ
	CUVRWLock(const CUVRWLock&);//��ʵ��
	CUVRWLock& operator =(const CUVRWLock&);//��ʵ��
};


class SUV_EXPORT CUVBarrier
{
public:
	CUVBarrier(int count)
	{
		uv_barrier_init(&barrier_, count);
	}
	~CUVBarrier(void)
	{
		uv_barrier_destroy(&barrier_);
	}
	void Wait()
	{
		uv_barrier_wait(&barrier_);
	}
private:
	uv_barrier_t barrier_;
private://private�У���ֹ���ƺ͸�ֵ
	CUVBarrier(const CUVBarrier&);//��ʵ��
	CUVBarrier& operator =(const CUVBarrier&);//��ʵ��
};

#endif
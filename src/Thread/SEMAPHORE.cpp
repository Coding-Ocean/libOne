#include<windows.h>
#include"common.h"
#include"thread/SEMAPHORE.h"
#include"thread/ReferenceType.h"




class SEMAPHORE::Impl : public ReferenceType{
public:
	Impl( int startCount, int maxCount ) : mMaxCount( maxCount ){
		mHandle = CreateSemaphore( NULL, startCount, mMaxCount, NULL );
	}
	~Impl(){
		CloseHandle( mHandle );
	}
	void decrease(){
		WaitForSingleObject( mHandle, INFINITE );
	}
	void increase( int value ){
		LONG prev;
		ReleaseSemaphore( mHandle, value, &prev );
		STRONG_ASSERT( prev + value <= mMaxCount );
	}
	HANDLE mHandle;
	int mMaxCount;
};

SEMAPHORE SEMAPHORE::create( int startCount, int maxCount ){
	STRONG_ASSERT( startCount >= 0 );
	if ( maxCount < startCount ){
		maxCount = startCount;
	}
	SEMAPHORE r;
	r.mImpl = new Impl( startCount, maxCount );
	return r;
}

void SEMAPHORE::decrease(){
	mImpl->decrease();
}

void SEMAPHORE::increase( int v ){
	STRONG_ASSERT( v > 0 );
	mImpl->increase( v );
}

#define TYPE SEMAPHORE
#include"thread/ReferenceTypeTemplate.h"





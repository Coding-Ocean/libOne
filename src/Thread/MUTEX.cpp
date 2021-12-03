#include<windows.h>
#include"common.h"
#include"thread/MUTEX.h"
#include"thread/ReferenceType.h"

//多重アンロック、多重ロック等々を検出するために、中身はセマフォにしておく。
//Release版でCriticalSectionに切り替えるのもいいかもしれない。
class MUTEX::Impl : public ReferenceType{
public:
	Impl() : mHandle( 0 ){
		mHandle = CreateSemaphore( NULL, 1, 1, NULL );
	}
	~Impl(){
		CloseHandle( mHandle );
	}
	void lock(){
		WaitForSingleObject( mHandle, INFINITE );
	}
	void unlock(){
		LONG prev;
		ReleaseSemaphore( mHandle, 1, &prev );
		STRONG_ASSERT( prev == 0 && "unlock() is called twice! Check lock-unlock pair." );
	}
	HANDLE mHandle;
};

MUTEX MUTEX::create(){
	MUTEX r;
	r.mImpl = new Impl();
	return r;
}

void MUTEX::lock(){
	mImpl->lock();
}

void MUTEX::unlock(){
	mImpl->unlock();
}

#define TYPE MUTEX
#include"thread/ReferenceTypeTemplate.h"





#include<windows.h>
#include"common.h"
#include"thread/EVENT.h"
#include"thread/ReferenceType.h"

class EVENT::Impl : public ReferenceType{
public:
	Impl( bool f ) : mHandle( 0 ){
		BOOL t = ( f ) ? TRUE : FALSE;
		mHandle = CreateEvent( NULL, TRUE, t, NULL );
	}
	~Impl(){
		CloseHandle( mHandle );
		mHandle = 0;
	}
	HANDLE mHandle;
};

EVENT EVENT::create( bool f ){
	EVENT r;
	r.mImpl = new Impl( f );
	return r;
}

void EVENT::wait() const {
	WaitForSingleObject( mImpl->mHandle, INFINITE );
}

bool EVENT::isSet() const {
	return ( WaitForSingleObject( mImpl->mHandle, 0 ) == WAIT_OBJECT_0 );
}

void EVENT::set(){
	SetEvent( mImpl->mHandle );
}

void EVENT::reset(){
	ResetEvent( mImpl->mHandle );
}

#define TYPE EVENT
#include"thread/ReferenceTypeTemplate.h"

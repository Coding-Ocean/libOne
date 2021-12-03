#include"common.h"
#include"thread/THREAD.h"
#include"thread/EVENT.h"
#include"thread/THREAD_MANAGER.h"

class THREAD::IMPL{
public:
	IMPL(){
		event = EVENT::create();
	}
	~IMPL(){
	}
	EVENT event;
};

THREAD::THREAD() : 
    Impl( 0 ){
}

void THREAD::start(){
	wait(); //前のがあれば待つ
	Impl = new IMPL;
    THREAD_MANAGER* th = THREAD_MANAGER::instance();
	th->add( this, &Impl->event ); //実行開始
}

void THREAD::wait(){
	if ( Impl ){
		Impl->event.wait();
		SAFE_DELETE( Impl );
	}
}

bool THREAD::isFinished(){
	bool r = false;
	if ( Impl ){
		if ( Impl->event.isSet() ){ //終わってる
			SAFE_DELETE( Impl );
			r = true;
		}
	}else{
		r = true;
	}
	return r;
}

THREAD::~THREAD(){
	STRONG_ASSERT( !Impl && "You must call wait() or isFinished() before destruction." );
}




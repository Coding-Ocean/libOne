#include<process.h>
#include"common.h"
#include"window.h"
#include"thread/THREAD_MANAGER.h"
#include"thread/THREAD.h"
#include"thread/EVENT.h"
#include"thread/THREAD_MANAGER.h"

THREAD_MANAGER::THREAD_MANAGER( int additionalThreadNumber ) : 
	mThreads( 0 ),
	mThreadNumber( 0 ),
	mEndRequest( false ),
	mCoreNumber( 1 ) { //最低でも1
	//別スレッドからの呼び出しは許さない
	WARNING(isMainThread(), "you must call from MAIN thread", "" );

	mQueueSemaphore = SEMAPHORE::create( 0, 0x7fffffff ); //最大値は無限
	mLock = MUTEX::create();

	//コア数取得
#ifdef NDEBUG //Debug版では自動では一個しか作らない。開発マシンで動いて他で動かない、を防ぐため。
	HANDLE process = GetCurrentProcess();
	DWORD processMask;
	DWORD systemMask;
	BOOL succeeded = GetProcessAffinityMask( process, (PDWORD_PTR)&processMask, (PDWORD_PTR)&systemMask );
	if ( succeeded != 0 ){ //失敗されても困るけどな...
		for ( int i = 1; i < 32; ++i ){
			if ( processMask & ( 1 << i ) ){
				++mCoreNumber;
			}
		}
	}
#endif
	//mThreadNumber = additionalThreadNumber + mCoreNumber + 1; //最低でもコア数+1(ローダ用)は作る
	mThreadNumber = additionalThreadNumber + mCoreNumber; //最低でもコア数は作る
	mThreads = new HANDLE[ mThreadNumber ];
	//起動
	for ( int i = 0; i < mThreadNumber; ++i ){
		unsigned dummy;
		mThreads[ i ] = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, execute, this, 0, &dummy ) );
		SetThreadPriority( mThreads[ i ], THREAD_PRIORITY_BELOW_NORMAL ); //優先度ちょっと下
	}
}

THREAD_MANAGER::~THREAD_MANAGER(){
	//終了待ち
	mLock.lock();
	mEndRequest = true;
	mLock.unlock();

	//スレッド終了待ち
	for ( int i = 0; i < mThreadNumber; ++i ){
		while ( true ){
			//終わらない間セマフォを増やしながらぐるぐる待つ。decreaseで待ってるのでincreaseしないと起きない。
			mQueueSemaphore.increase(); //キューに何か入っているように見せかけて起こす
			if ( WAIT_OBJECT_0 == WaitForSingleObject( mThreads[ i ], 1 ) ){ 
				CloseHandle( mThreads[ i ] );
				mThreads[ i ] = 0;
				break;
			}
		}
	}
	SAFE_DELETE_ARRAY( mThreads );
}

int THREAD_MANAGER::getCoreNumber() const {
	return mCoreNumber;
}

unsigned WINAPI THREAD_MANAGER::execute( void* p ){
	THREAD_MANAGER* m = reinterpret_cast< THREAD_MANAGER* >( p );
	m->execute();
	return 0;
}

void THREAD_MANAGER::execute(){
	while ( true ){
		mQueueSemaphore.decrease(); //キュー追加を待つ

		mLock.lock();
		if ( mEndRequest ){
			mLock.unlock();
			break;
		}
		Entry e = mQueue.front();
		mQueue.pop_front(); //キューから取り出す
		mLock.unlock();

		( *( e.mThread ) )(); //実行
		e.mFinished->set(); //終了フラグ立てる
	}
}

void THREAD_MANAGER::add( THREAD* t, EVENT* finishFlag ){
	Entry e( t, finishFlag );

	mLock.lock();
	mQueue.push_back( e );
	mLock.unlock();

	mQueueSemaphore.increase(); //足しました
}



 
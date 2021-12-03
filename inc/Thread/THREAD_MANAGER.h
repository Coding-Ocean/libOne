#pragma once

#include<windows.h>
#include<list>
#include"SINGLETON.h"
#include"SEMAPHORE.h"
#include"MUTEX.h"
class THREAD;
class EVENT;
using namespace std;

class THREAD_MANAGER : public SINGLETON< THREAD_MANAGER >{
public:
	THREAD_MANAGER( int additionalThreadNumber );
	~THREAD_MANAGER();
	//コア数取得(論理コア数なので、実体とは違う可能性がある)
	int getCoreNumber() const;

	////以下ユーザが呼ぶことはない
    static unsigned WINAPI execute( void* p );
    void execute();
	struct Entry{
		Entry() : 
            mThread( 0 ), 
            mFinished( 0 ){
        }
		Entry( THREAD* t, EVENT* e ) : 
            mThread( t ), 
            mFinished( e ){
        }
		~Entry(){ 
			mThread = 0;
			mFinished = 0;
		}
		THREAD* mThread;
		EVENT* mFinished;
	};
    void add( THREAD* t, EVENT* finishFlag );
private:
	HANDLE* mThreads; //スレッドハンドル配列
	int mThreadNumber;
	bool mEndRequest;
	SEMAPHORE mQueueSemaphore;
	MUTEX mLock;
	list< Entry > mQueue; //要求待ち行列
	int mCoreNumber;
};

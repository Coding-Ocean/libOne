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
	//�R�A���擾(�_���R�A���Ȃ̂ŁA���̂Ƃ͈Ⴄ�\��������)
	int getCoreNumber() const;

	////�ȉ����[�U���ĂԂ��Ƃ͂Ȃ�
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
	HANDLE* mThreads; //�X���b�h�n���h���z��
	int mThreadNumber;
	bool mEndRequest;
	SEMAPHORE mQueueSemaphore;
	MUTEX mLock;
	list< Entry > mQueue; //�v���҂��s��
	int mCoreNumber;
};

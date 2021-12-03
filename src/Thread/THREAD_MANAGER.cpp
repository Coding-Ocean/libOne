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
	mCoreNumber( 1 ) { //�Œ�ł�1
	//�ʃX���b�h����̌Ăяo���͋����Ȃ�
	WARNING(isMainThread(), "you must call from MAIN thread", "" );

	mQueueSemaphore = SEMAPHORE::create( 0, 0x7fffffff ); //�ő�l�͖���
	mLock = MUTEX::create();

	//�R�A���擾
#ifdef NDEBUG //Debug�łł͎����ł͈�������Ȃ��B�J���}�V���œ����đ��œ����Ȃ��A��h�����߁B
	HANDLE process = GetCurrentProcess();
	DWORD processMask;
	DWORD systemMask;
	BOOL succeeded = GetProcessAffinityMask( process, (PDWORD_PTR)&processMask, (PDWORD_PTR)&systemMask );
	if ( succeeded != 0 ){ //���s����Ă����邯�ǂ�...
		for ( int i = 1; i < 32; ++i ){
			if ( processMask & ( 1 << i ) ){
				++mCoreNumber;
			}
		}
	}
#endif
	//mThreadNumber = additionalThreadNumber + mCoreNumber + 1; //�Œ�ł��R�A��+1(���[�_�p)�͍��
	mThreadNumber = additionalThreadNumber + mCoreNumber; //�Œ�ł��R�A���͍��
	mThreads = new HANDLE[ mThreadNumber ];
	//�N��
	for ( int i = 0; i < mThreadNumber; ++i ){
		unsigned dummy;
		mThreads[ i ] = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, execute, this, 0, &dummy ) );
		SetThreadPriority( mThreads[ i ], THREAD_PRIORITY_BELOW_NORMAL ); //�D��x������Ɖ�
	}
}

THREAD_MANAGER::~THREAD_MANAGER(){
	//�I���҂�
	mLock.lock();
	mEndRequest = true;
	mLock.unlock();

	//�X���b�h�I���҂�
	for ( int i = 0; i < mThreadNumber; ++i ){
		while ( true ){
			//�I���Ȃ��ԃZ�}�t�H�𑝂₵�Ȃ��炮�邮��҂Bdecrease�ő҂��Ă�̂�increase���Ȃ��ƋN���Ȃ��B
			mQueueSemaphore.increase(); //�L���[�ɉ��������Ă���悤�Ɍ��������ċN����
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
		mQueueSemaphore.decrease(); //�L���[�ǉ���҂�

		mLock.lock();
		if ( mEndRequest ){
			mLock.unlock();
			break;
		}
		Entry e = mQueue.front();
		mQueue.pop_front(); //�L���[������o��
		mLock.unlock();

		( *( e.mThread ) )(); //���s
		e.mFinished->set(); //�I���t���O���Ă�
	}
}

void THREAD_MANAGER::add( THREAD* t, EVENT* finishFlag ){
	Entry e( t, finishFlag );

	mLock.lock();
	mQueue.push_back( e );
	mLock.unlock();

	mQueueSemaphore.increase(); //�����܂���
}



 
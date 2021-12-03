#ifndef SEMAPHORE_HD
#define SEMAPHORE_HD

///���܂����l�����������Ƀ��b�N�ł���MUTEX�B
class SEMAPHORE{
public:
	///�����̏����l�Ő����BmaxCount��0����startCount�ɂȂ�B
	static SEMAPHORE create( int startCount, int maxCount = 0 );
	///�J�E���g��1���炻���Ƃ���B���ł�0���Ɩ��炳���B
	void decrease();
	///�J�E���g�𑝂₷�B�������ȗ������1�ÂB
	void increase( int value = 1 );

	SEMAPHORE();
	~SEMAPHORE();
	SEMAPHORE( const SEMAPHORE& );
	operator void*() const;
	SEMAPHORE& operator=( const SEMAPHORE& );
	void release();
	bool operator==( const SEMAPHORE& ) const;
	bool operator!=( const SEMAPHORE& ) const;
private:
	class Impl;
	Impl* mImpl;
};




#endif

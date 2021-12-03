#ifndef SEMAPHORE_HD
#define SEMAPHORE_HD

///決まった人数だけ同時にロックできるMUTEX。
class SEMAPHORE{
public:
	///引数の初期値で生成。maxCountは0だとstartCountになる。
	static SEMAPHORE create( int startCount, int maxCount = 0 );
	///カウントを1減らそうとする。すでに0だと眠らされる。
	void decrease();
	///カウントを増やす。引数を省略すれば1づつ。
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

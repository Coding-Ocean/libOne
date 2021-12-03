#ifndef MUTEX_HD
#define MUTEX_HD

class MUTEX{
public:
	static MUTEX create();
	void lock();
	void unlock();

	MUTEX();
	~MUTEX();
	MUTEX( const MUTEX& );
	operator void*() const;
	MUTEX& operator=( const MUTEX& );
	void release();
	bool operator==( const MUTEX& ) const;
	bool operator!=( const MUTEX& ) const;
private:
	class Impl;
	Impl* mImpl;
};

#endif

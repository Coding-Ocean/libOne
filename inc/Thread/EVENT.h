#ifndef EVENT_HD
#define EVENT_HD

class EVENT{
public:
	static EVENT create( bool initialiState = false );
	///true�ɂ����̂�҂�
	void wait() const;
	///true�ɂ���
	void set();
	///false�ɂ���
	void reset();
	///true?����Ƃ�false?
	bool isSet() const;

	EVENT();
	~EVENT();
	EVENT( const EVENT& );
	operator void*() const;
	EVENT& operator=( const EVENT& );
	void release();
	bool operator==( const EVENT& ) const;
	bool operator!=( const EVENT& ) const;
private:
	class Impl;
	Impl* mImpl;
};

#endif

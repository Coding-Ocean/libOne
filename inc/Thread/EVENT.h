#ifndef EVENT_HD
#define EVENT_HD

class EVENT{
public:
	static EVENT create( bool initialiState = false );
	///true‚É‚³‚ê‚é‚Ì‚ð‘Ò‚Â
	void wait() const;
	///true‚É‚·‚é
	void set();
	///false‚É‚·‚é
	void reset();
	///true?‚»‚ê‚Æ‚àfalse?
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

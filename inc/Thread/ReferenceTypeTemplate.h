//����̓N���X�������l�����ŁA���[�U���C���N���[�h���邱�Ƃ͂Ȃ��B
//�Q�ƃJ�E���g�^�̃N���X�ł͕K�������Ă���֐��Ƃ������̂�����A
//����������̂��y�ɂ���B
//#define TYPE ActualType
//�Ə����Ă���C���N���[�h���邱�ƁB�������ActualType�͖{���̌^�̖��O���B

TYPE::TYPE() : mImpl( 0 ){
}

TYPE::TYPE( const TYPE& o ) : mImpl( o.mImpl ){
	if ( mImpl ){
		mImpl->refer();
	}
}

TYPE::~TYPE(){
	release();
}

void TYPE::release(){
	if ( mImpl ){
		mImpl->release();
		if ( mImpl->referenceCount() == 0 ){
			SAFE_DELETE( mImpl );
		}
	}
	mImpl = 0;
}

TYPE& TYPE::operator=( const TYPE& o ){
	release();
	mImpl = o.mImpl;
	if ( mImpl ){
		mImpl->refer();
	}
	return *this;
}

bool TYPE::operator==( const TYPE& o ) const {
	return ( mImpl == o.mImpl );
}

bool TYPE::operator!=( const TYPE& o ) const {
	return ( mImpl != o.mImpl );
}

TYPE::operator void*() const {
	return ( mImpl == 0 ) ? 0 : reinterpret_cast< void* >( ~0 );
}

#undef TYPE

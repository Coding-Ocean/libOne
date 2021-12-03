#ifndef REFERENCETYPE_HD
#define REFERENCETYPE_HD

//�Q�ƃJ�E���g�����^����鎞�Ɋy�ɂ��邽�߂̊��N���X
class ReferenceType{
public:
	ReferenceType() : mReferenceCount( 1 ){
	}
	void refer(){
		++mReferenceCount;
	}
	void release(){
		--mReferenceCount;
	}
	int referenceCount() const {
		return mReferenceCount;
	}
protected:
	~ReferenceType(){
	}
private:
	int mReferenceCount;
};

#endif

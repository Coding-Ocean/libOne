#ifndef REFERENCETYPE_HD
#define REFERENCETYPE_HD

//参照カウントを持つ型を作る時に楽にするための基底クラス
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

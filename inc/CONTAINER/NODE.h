#ifndef NODE_HD
#define NODE_HD

#include"VECTOR.h"
#include"MATRIX.h"
#include"NAME.h"
class BATCH;
class ANIMATION_NODE;
struct ID3D11Buffer;
class SHADER;

class NODE{
public:
    NODE();
    ~NODE();
    void setName( const NAME& name );
    void setBatch( const BATCH* batch );
    void setScale( const VECTOR& scale );
    void setRotate( const VECTOR& rotate );
    void setTranslate( const VECTOR& translate );
    void setToOriginWorld( const MATRIX& toOriginWorld );
    void setNumChildren( int num );
    void setChild( int index, NODE* node );
    void setIndex( int idx );

    void setAnimationNode( const ANIMATION_NODE* an );
    void copyAnimNodeToAnimNode2();//モーフアニメーション用
    void update( double frameNumber, double frameNumber2, double weight2, const MATRIX& parentWorld, MATRIX* worldArray );
    void draw( SHADER* shader, MATRIX* worldArray );

    const NAME& name() const;

private:
    NAME Name;
    const BATCH* Batch;
    VECTOR Scale;
    VECTOR Rotate;
    VECTOR Translate;
    MATRIX ToOriginWorld;
    //子供NODEポインタの配列
    NODE** Children;
    int NumChildren;
    const ANIMATION_NODE* AnimationNode;
    const ANIMATION_NODE* AnimationNode2;//モーフアニメーション用

    int Idx;
};

#endif


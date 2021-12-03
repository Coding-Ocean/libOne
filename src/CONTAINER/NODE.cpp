#include"common.h"
#include"graphic.h"
#include"CONTAINER/NODE.h"
#include"CONTAINER/NODE_ORIGIN.h"
#include"CONTAINER/BATCH.h"
#include"CONTAINER/ANIMATION_NODE.h"
#include"SHADER/SHADER.h"

NODE::NODE() :
Name(),
Batch( 0 ),
Scale( 1.0f, 1.0f, 1.0f ),
Rotate( 0.0f, 0.0f, 0.0f ),
Translate( 0.0f, 0.0f, 0.0f ),
Children( 0 ),
NumChildren( 0 ),
AnimationNode( 0 ),
AnimationNode2( 0 ){
}

NODE::~NODE(){
    SAFE_DELETE_ARRAY( Children );
    Batch = 0;
    AnimationNode = 0;
    AnimationNode2 = 0;
}

void NODE::setName( const NAME& name ){
    Name = name;
}

void NODE::setScale( const VECTOR& scale ){
    Scale = scale;
}

void NODE::setRotate( const VECTOR& rotate ){
    Rotate = rotate;
}

void NODE::setTranslate( const VECTOR& translate ){
    Translate = translate;
}

void NODE::setToOriginWorld( const MATRIX& toOriginWorld ){
    ToOriginWorld = toOriginWorld;
}

void NODE::setBatch( const BATCH* batch ){
    Batch = batch;
}

void NODE::setNumChildren( int num ){
    NumChildren = num;
    if( NumChildren ){
        if ( Children ){
            SAFE_DELETE_ARRAY( Children );
        }
        Children = new NODE*[ NumChildren ];
        //念のため0埋め。バグがあれば死んでくれる
        for ( int i = 0; i < NumChildren; ++i ){
            Children[ i ] = 0;
        }
    }
}

void NODE::setChild( int i, NODE* node ){
    WARNING( ( i >= NumChildren ), "添え字オーバー", "" );
    Children[ i ] = node;
}

void NODE::update( double frameNumber, double frameNumber2, double weight2, const MATRIX& parentWorld, MATRIX* worldArray  ){

    if ( AnimationNode ){
        AnimationNode->update( frameNumber, &Scale, &Rotate, &Translate );
    }

    //次のアニメーションがセットされた場合の移行（morph）処理
    //上の新しくセットされたアニメーションのScale,Rotate,Translateと
    //現在のアニメーションのScale,Rotate,TranslateとをWeight2の割合で合成
	if( weight2 > 0.0 ){
        if( AnimationNode2 ){
		    AnimationNode2->update2( frameNumber2, weight2, &Scale, &Rotate, &Translate );
        }
    }

    worldArray[ Idx ].identity();
    worldArray[ Idx ].mulTranslate( Translate );
    worldArray[ Idx ].mulRotateY( Rotate.y );
    worldArray[ Idx ].mulRotateZ( Rotate.z );
    worldArray[ Idx ].mulRotateX( Rotate.x );
    worldArray[ Idx ].mulScaling( Scale );

    worldArray[ Idx ] = parentWorld * worldArray[ Idx ];


    for ( int i = 0; i < NumChildren; ++i ){
        Children[ i ]->update( frameNumber, frameNumber2, weight2, worldArray[ Idx ], worldArray  );
    }

    worldArray[ Idx ] = worldArray[ Idx ] * ToOriginWorld;
}

void NODE::draw(  SHADER* shader, MATRIX* worldArray ){
    if ( Batch ){
		if( Batch->vertexType() == VERTEX_TYPE::PNTWW ){
			Batch->drawSkin( shader );
		}
		else{
			Batch->draw( shader, worldArray[ Idx ] );
		}
    }
}
 
const NAME& NODE::name() const{
    return Name;
}

void NODE::setAnimationNode( const ANIMATION_NODE* an ){
    AnimationNode = an;
}

void NODE::setIndex( int idx ){
    Idx = idx;
}

void NODE::copyAnimNodeToAnimNode2(){
	AnimationNode2 = AnimationNode;
}
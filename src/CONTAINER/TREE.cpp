#include<d3d11.h>
#include"common.h"
#include"graphic.h"
#include"MATRIX.h"
#include"CONTAINER/TREE.h"
#include"CONTAINER/NODE.h"
#include"CONTAINER/TREE_ORIGIN.h"
#include"CONTAINER/NODE_ORIGIN.h"
#include"CONTAINER/ANIMATION.h"
#include"SHADER/SHADER.h"

TREE::TREE():
    Nodes( 0 ),
    WorldArray( 0 ),
    CBWorldArray( 0 ),
    NumNodes( 0 ),
    FrameNumber( 0 ),
    AnimationSpeed( 0.5 ),
    FrameNumber2( 0 ),
    AnimationSpeed2( 0.5 ),
    Weight2(0),
    Decrease2( 0 ){
}

TREE::TREE( const TREE_ORIGIN* treeOrigin ):
    Nodes( 0 ),
    WorldArray( 0 ),
    CBWorldArray( 0 ),
    NumNodes( 0 ),
    FrameNumber( 0 ),
    AnimationSpeed( 0.5 ),
    FrameNumber2( 0 ),
    AnimationSpeed2( 0.5 ),
    Weight2(0),
    Decrease2( 0 ){
    Create( treeOrigin );
}

TREE::~TREE(){
    Destroy();
}

void TREE::operator=( const TREE_ORIGIN*  treeOrigin ){
    NumNodes = 0;
    FrameNumber = 0;
    AnimationSpeed = 0.5;
    FrameNumber2 = 0;
    AnimationSpeed2 = 0.5;
    Weight2 = 0;
    Decrease2 = 0;
    Destroy();
    Create( treeOrigin );
}

void TREE::Destroy(){
    SAFE_RELEASE( CBWorldArray );
    SAFE_DELETE_ARRAY( WorldArray );
    SAFE_DELETE_ARRAY( Nodes );
}

void TREE::Create( const TREE_ORIGIN* treeOrigin ){
    //TREE_ORIGIN から クローンである TREE を作成
    NumNodes = treeOrigin->numNodeOrigins();
    Nodes = new NODE[ NumNodes ];
    for ( int i = 0; i < NumNodes; i++ ){
        //名前を短くする
        NODE& node = Nodes[ i ];
        const NODE_ORIGIN* nodeOrigin = treeOrigin->nodeOrigins( i );
        //NODE_ORIGIN を NODE にコピー
        node.setName( nodeOrigin->name() );
        node.setBatch( nodeOrigin->batch() );
        node.setTranslate( nodeOrigin->translate() );
        node.setRotate( nodeOrigin->rotate() );
        node.setScale( nodeOrigin->scale() );
        node.setToOriginWorld( nodeOrigin->toOriginWorld() );
        node.setIndex( i );////////
        node.setAnimationNode( 0 );
        //ツリー構造を「長男,弟形式」から「子配列形式」へ変換する
        //まず子供の数を数える（始めは長男、その後は弟のインデックスをたどっていく）
        int nodeIdx = nodeOrigin->chounanIdx();
        int numChildren = 0;
        while ( nodeIdx >= 0 ){
            numChildren++;
            nodeIdx = treeOrigin->nodeOrigins( nodeIdx )->otoutoIdx();
        }
        //子供の数を設定（関数内ではポインタ配列のアロケートもしている）
        node.setNumChildren( numChildren );
        //子のポインタ配列を充填する
        nodeIdx = nodeOrigin->chounanIdx();
        int j = 0;
        while ( nodeIdx >= 0 ){
            node.setChild( j, Nodes + nodeIdx );
            nodeIdx = treeOrigin->nodeOrigins( nodeIdx )->otoutoIdx();
            j++;
        }
    }
    //ワールドマトリックス配列とコンスタントバッファを用意
    WorldArray = new MATRIX[ NumNodes ];
    createConstantBuffer( sizeof( MATRIX ) * NumNodes, &CBWorldArray );
}

void TREE::setAnimation( const ANIMATION* a, double animationSpeed  ){
    for( int i = 0; i < NumNodes; ++i ){
        const ANIMATION_NODE* an = a->animationNode( Nodes[ i ].name() );
        Nodes[ i ].setAnimationNode( an ); //0でも気にせずセット。むしろしないとまずい。
    }
    FrameNumber = 0.0; //巻き戻し
    AnimationSpeed = animationSpeed;
}

void TREE::setAnimationSpeed( double animationSpeed ){
    AnimationSpeed = animationSpeed;
}

void TREE::setNextAnimation( const ANIMATION* a, double numMorphFrames, double startFrame, double animationSpeed ){
    for( int i = 0; i < NumNodes; ++i ){
        Nodes[ i ].copyAnimNodeToAnimNode2();//現在のアニメーションはanimationNode2へコピーする
        const ANIMATION_NODE* an = a->animationNode( Nodes[ i ].name() );
        Nodes[ i ].setAnimationNode( an );//次のアニメーションをanimationNodeへセット
    }
    FrameNumber2 = FrameNumber;//現在の時刻もFrameNumber2へコピー
    AnimationSpeed2 = AnimationSpeed;
    Weight2 = 1.0;
    Decrease2 = -1.0 / numMorphFrames;
    FrameNumber = startFrame; //次のアニメーション用に時刻をリセット
    AnimationSpeed = animationSpeed;
}

void TREE::update( const MATRIX& world ){
    Nodes[ 0 ].update( FrameNumber, FrameNumber2, Weight2, world, WorldArray );
    FrameNumber += AnimationSpeed;
    if( Weight2 > 0.0 ){
        FrameNumber2 += AnimationSpeed2;
        Weight2 += Decrease2;
    }
}

void TREE::draw( SHADER* shader ){
    shader->setWorldArray( CBWorldArray, WorldArray );
    for( int i = 0; i < NumNodes; i++ ){
       Nodes[ i ].draw( shader, WorldArray );
    }
}

NODE* TREE::node( const char* name ){
    for( int i = 0; i < NumNodes; ++i ){
        if( Nodes[ i ].name() == name ) {
            return &Nodes[ i ];
        }
    }
    return 0;
}

double TREE::frameNumber(){
    return FrameNumber;
}

const MATRIX* TREE::nodeWorld( const char* name ) const{
    int i;
    for( i = 0; i < NumNodes; ++i ){
        if( Nodes[ i ].name() == name ) {
            break;
        }
    }
    WARNING( i == NumNodes, name, "このノードはありまっせん" );
    return &WorldArray[ i ];
}

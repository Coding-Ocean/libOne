#include"common.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/ANIMATION.h"
#include"CONTAINER/ANIMATION_NODE.h"

ANIMATION::ANIMATION( FILE_BUFFER& fb ):
    Name( fb.readString() ),
    AnimationNodes( 0 ),
    NumAnimationNodes( 0 ){
    //NumAnimationNodes
    NumAnimationNodes = fb.readInt();
    fb.readOnAssumption( "{" );
    //AnimationNodesアロケート
    AnimationNodes = new ANIMATION_NODE[ NumAnimationNodes ];
    for( int i = 0; i < NumAnimationNodes; i++ ){
        AnimationNodes[ i ].set( fb );
    }
}

ANIMATION::~ANIMATION(){
    SAFE_DELETE_ARRAY( AnimationNodes );
    NumAnimationNodes = 0;
}    
    
const NAME& ANIMATION::name() const{
    return Name;
}

//treeのnodeに対応するAnimationNodeを見つける
const ANIMATION_NODE* ANIMATION::animationNode( const NAME& name ) const{
    for( int i = 0; i < NumAnimationNodes; ++i ){
        if( AnimationNodes[ i ].name() == name ){
            return &AnimationNodes[ i ];
        }
    }
    return 0;
}

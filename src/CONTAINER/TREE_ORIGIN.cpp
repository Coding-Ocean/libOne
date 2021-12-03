#include"common.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/TREE_ORIGIN.h"
#include"CONTAINER/NODE_ORIGIN.h"
#include"CONTAINER/CONTAINER.h"

TREE_ORIGIN::TREE_ORIGIN( FILE_BUFFER& fb, const CONTAINER* container ):
    Idx( 0 ),
    NodeOrigins( 0 ),
    NumNodeOrigins( 0 ),
    Name( fb.readString() ) {
    
    //ノード数を求め、領域を確保
    fb.readOnAssumption( "{" ); //最初の'{' を読み込む
    const char* bp = fb.bufferPointer();//bpは'{'の次の文字をさしている
    NumNodeOrigins = 1;
    int cnt = 1;
    while( cnt > 0 ){
        if( *bp == '{' ){
            NumNodeOrigins++;
            cnt++;
        }
        else if( *bp == '}' ){
            cnt--;
        }
        bp++;
    }
    NodeOrigins = new NODE_ORIGIN[ NumNodeOrigins ];
    
    //各NodeOriginへデータを読み込む（再起関数）
    SetNodeOrigin( fb, container );
}

TREE_ORIGIN::~TREE_ORIGIN(){
    SAFE_DELETE_ARRAY( NodeOrigins );
}

void TREE_ORIGIN::SetNodeOrigin( FILE_BUFFER& fb, const CONTAINER* container ){
    //現在のインデックスを持っておく（再帰呼び出しして、帰ってきたときに使用）
    int idx = Idx;

    fb.readOnAssumption( "n" ); //'{'の次の次の文字（'n'のはず）
    while( fb != "{" && fb != "}"  && !fb.end() ){
        //node name
        if(      fb == "n" ){
            NodeOrigins[ idx ].Name = fb.readString();
        }
        //batchがある場合の処理
        else if( fb == "b" ){
            NodeOrigins[ idx ].Batch = container->batch( fb.readString() );
        }
        //translate dataがある場合の処理
        else if( fb == "t" ){
            NodeOrigins[ idx ].Translate.x = fb.readFloat();
            NodeOrigins[ idx ].Translate.y = fb.readFloat();
            NodeOrigins[ idx ].Translate.z = fb.readFloat();
        }
        //rotate dataがある場合の処理
        else if( fb == "r" ){
            NodeOrigins[ idx ].Rotate.x = fb.readFloat();
            NodeOrigins[ idx ].Rotate.y = fb.readFloat();
            NodeOrigins[ idx ].Rotate.z = fb.readFloat();
        }
        //scale dataがある場合の処理
        else if( fb == "s" ){
            NodeOrigins[ idx ].Scale.x = fb.readFloat();
            NodeOrigins[ idx ].Scale.y = fb.readFloat();
            NodeOrigins[ idx ].Scale.z = fb.readFloat();
        }
        //ピボットポイントを原点に持っていくマトリックス
        else if( fb == "m" ){
            NodeOrigins[ idx ].ToOriginWorld.identity();
            NodeOrigins[ idx ].ToOriginWorld._11 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._12 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._13 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._14 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._21 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._22 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._23 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._24 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._31 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._32 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._33 = fb.readFloat();
            NodeOrigins[ idx ].ToOriginWorld._34 = fb.readFloat();
        }

        fb.readString();
    }

    //次のノードに進める
    Idx++;

    //自分の子供（長男）がいた場合の処理
    if( fb == "{" ){
        NodeOrigins[ idx ].ChounanIdx = Idx;
        SetNodeOrigin( fb, container );
    }

    //自分の弟がいた場合の処理
    if( idx != 0 ){//ルートノードでなければ（ルートに弟はいない）
        fb.readString();
        if( fb == "{" ){
            NodeOrigins[ idx ].OtoutoIdx = Idx;
            SetNodeOrigin( fb, container );
        }
    }
}

const NODE_ORIGIN* TREE_ORIGIN::nodeOrigins( int idx ) const {
    return &NodeOrigins[ idx ];
}

const int TREE_ORIGIN::numNodeOrigins() const {
    return NumNodeOrigins;
}

const NAME& TREE_ORIGIN::name() const{
    return Name;
}



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
    
    //�m�[�h�������߁A�̈���m��
    fb.readOnAssumption( "{" ); //�ŏ���'{' ��ǂݍ���
    const char* bp = fb.bufferPointer();//bp��'{'�̎��̕����������Ă���
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
    
    //�eNodeOrigin�փf�[�^��ǂݍ��ށi�ċN�֐��j
    SetNodeOrigin( fb, container );
}

TREE_ORIGIN::~TREE_ORIGIN(){
    SAFE_DELETE_ARRAY( NodeOrigins );
}

void TREE_ORIGIN::SetNodeOrigin( FILE_BUFFER& fb, const CONTAINER* container ){
    //���݂̃C���f�b�N�X�������Ă����i�ċA�Ăяo�����āA�A���Ă����Ƃ��Ɏg�p�j
    int idx = Idx;

    fb.readOnAssumption( "n" ); //'{'�̎��̎��̕����i'n'�̂͂��j
    while( fb != "{" && fb != "}"  && !fb.end() ){
        //node name
        if(      fb == "n" ){
            NodeOrigins[ idx ].Name = fb.readString();
        }
        //batch������ꍇ�̏���
        else if( fb == "b" ){
            NodeOrigins[ idx ].Batch = container->batch( fb.readString() );
        }
        //translate data������ꍇ�̏���
        else if( fb == "t" ){
            NodeOrigins[ idx ].Translate.x = fb.readFloat();
            NodeOrigins[ idx ].Translate.y = fb.readFloat();
            NodeOrigins[ idx ].Translate.z = fb.readFloat();
        }
        //rotate data������ꍇ�̏���
        else if( fb == "r" ){
            NodeOrigins[ idx ].Rotate.x = fb.readFloat();
            NodeOrigins[ idx ].Rotate.y = fb.readFloat();
            NodeOrigins[ idx ].Rotate.z = fb.readFloat();
        }
        //scale data������ꍇ�̏���
        else if( fb == "s" ){
            NodeOrigins[ idx ].Scale.x = fb.readFloat();
            NodeOrigins[ idx ].Scale.y = fb.readFloat();
            NodeOrigins[ idx ].Scale.z = fb.readFloat();
        }
        //�s�{�b�g�|�C���g�����_�Ɏ����Ă����}�g���b�N�X
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

    //���̃m�[�h�ɐi�߂�
    Idx++;

    //�����̎q���i���j�j�������ꍇ�̏���
    if( fb == "{" ){
        NodeOrigins[ idx ].ChounanIdx = Idx;
        SetNodeOrigin( fb, container );
    }

    //�����̒킪�����ꍇ�̏���
    if( idx != 0 ){//���[�g�m�[�h�łȂ���΁i���[�g�ɒ�͂��Ȃ��j
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



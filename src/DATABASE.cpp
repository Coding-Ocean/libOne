#include<stdio.h>
#include"common.h"
#include"FILE_BUFFER.h"
#include"DATABASE.h"
DATABASE::DATABASE(){
}
DATABASE::DATABASE( const char* fileName ){
    Create( fileName );
}
void DATABASE::operator=( const char* fileName ){
    Destroy();
    Create( fileName );
}
DATABASE::~DATABASE(){
    Destroy();
}
void DATABASE::Create(const char* fileName) {
    FILE_BUFFER fb(fileName);
    //�f�[�^���𐔂���
    //�f�[�^���ƃf�[�^����ʂ����J�E���g�A�b�v
    NumData = 0;
    while (fb.readString()) {
        NumData++;
    }
    WARNING(NumData%2, "�f�[�^���ƃf�[�^���΂ɂȂ��Ă��Ȃ�", fileName);
    //�Q�Ŋ���ƃf�[�^���ƂȂ�
    NumData /= 2;
    //�|�C���^�z��̊m��
    Data = new DATA_ELEM * [NumData]();
    //�f�[�^�ǂݍ���
    fb.restart();
    int dataIdx = 0;
    while (dataIdx < NumData) {
        Data[dataIdx] = new DATA_ELEM(fb);
        dataIdx++;
    }
}
void DATABASE::Destroy(){
    for( int i = 0; i < NumData; i++ ){
        SAFE_DELETE( Data[ i ] );
    }
    SAFE_DELETE_ARRAY( Data );
    NumData = 0;
}
DATA_ELEM& DATABASE::data( const char* name ){
    int i;
    for( i = 0; i < NumData; i++ ){
        if( Data[ i ]->name() == name ){
            break;
        }
    }
    WARNING( i == NumData, "Data not found", name );
    return *Data[ i ];
}

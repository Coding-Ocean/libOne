#include "COLOR.h"

float toDegree = 180.0f / 3.141592f;
//�F��h�A�ʓxS�A���xV ���@RGB�ɕϊ�
COLOR hsv_to_rgb( float h, float s, float v ){
    float max = v;
    float min = max  - ( (s / 255) * max );
    float r=1,g=1,b=1;
    //���W�A���l���p�x�ɕϊ�
    h *= toDegree;
    //0�`360�̒l�ɕϊ�
    if( h <   0 ) h = h - int( h / 360 ) * 360 + 360;
    if( h > 360 ) h = h - int( h / 360 ) * 360;
    //60�x���Ƃ̏���
    if( h <= 60 ){ //0�`60
        r = max;
        g = (           h / 60 ) * ( max - min ) + min;
        b = min;
    }else
    if( h <= 120 ){ //60�`120
        r = ( ( 120 - h ) / 60 ) * ( max - min ) + min;
        g = max;
        b = min;
    }else
    if( h <= 180 ){ //120�`180
        r = min;
        g = max;
        b = ( ( h - 120 ) / 60 ) * ( max - min ) + min;
    }else
    if( h <= 240 ){ //180�`240
        r = min;
        g = ( ( 240 - h ) / 60 ) * ( max - min ) + min;
        b = max;
    }else
    if( h <= 300 ){ //240�`300
        r = ( ( h - 240 ) / 60 ) * ( max - min ) + min;
        g = min;
        b = max;
    }else
    if( h <= 360 ){ //300�`360
        r = max;
        g = min;
        b = ( ( 360 - h ) / 60 ) * ( max - min ) + min;
    }

    return COLOR( float( r / 255 ),float( g / 255 ),float( b / 255 ) );
}



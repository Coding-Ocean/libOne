#include"mathUtil.h"
#include"COLOR.h"

//色相h、彩度S、明度V を　RGBに変換
COLOR hsv_to_rgb( float h, float s, float v ){
    float max = v;
    float min = max  - ( (s / 255) * max );
    float r=0,g=0,b=0;
    if (AngleMode == RADIANS) {
        //ラジアン値を角度に変換
        h *= TO_DEG;
    }
    //0～360の値に変換
    if( h <   0 ) h = h - int( h / 360 ) * 360 + 360;
    if( h > 360 ) h = h - int( h / 360 ) * 360;
    //60度ごとの処理
    if( h <= 60 ){ //0～60
        r = max;
        g = (           h / 60 ) * ( max - min ) + min;
        b = min;
    }else
    if( h <= 120 ){ //60～120
        r = ( ( 120 - h ) / 60 ) * ( max - min ) + min;
        g = max;
        b = min;
    }else
    if( h <= 180 ){ //120～180
        r = min;
        g = max;
        b = ( ( h - 120 ) / 60 ) * ( max - min ) + min;
    }else
    if( h <= 240 ){ //180～240
        r = min;
        g = ( ( 240 - h ) / 60 ) * ( max - min ) + min;
        b = max;
    }else
    if( h <= 300 ){ //240～300
        r = ( ( h - 240 ) / 60 ) * ( max - min ) + min;
        g = min;
        b = max;
    }else
    if( h <= 360 ){ //300～360
        r = max;
        g = min;
        b = ( ( 360 - h ) / 60 ) * ( max - min ) + min;
    }

    return COLOR(r, g, b);
}



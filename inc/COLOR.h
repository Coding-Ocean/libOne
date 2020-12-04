#pragma once

struct COLOR{
    float r, g, b, a;
    COLOR() {
        r = 255; g = 255; b = 255; a = 255;
    }
    COLOR( float r, float g, float b, float a=255 ){
        this->r = r; this->g = g; this->b = b; this->a = a;
    }
    COLOR( unsigned c ){
	    a = float(c>>24);
	    r = float((c&0x00ff0000)>>16);
	    g = float((c&0x0000ff00)>> 8);
	    b = float(c&0x000000ff);
    }
    COLOR operator*( float f )const{
        return( COLOR( r * f, g * f, b * f, a ) );
    }
};

//�F��h�A�ʓxS�A���xV ���@RGB�ɕϊ�
COLOR hsv_to_rgb( float h, float s=255, float v=255 );




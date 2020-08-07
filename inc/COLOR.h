#ifndef COLOR_HD
#define COLOR_HD

struct COLOR{
    float r, g, b, a;
    COLOR( float r=1, float g=1, float b=1, float a=1 ){
        this->r = r; this->g = g; this->b = b; this->a = a;
    }
    COLOR( unsigned c ){
	    a = (c>>24)/255.0f;
	    r = ((c&0x00ff0000)>>16)/255.0f;
	    g = ((c&0x0000ff00)>> 8)/255.0f;
	    b = (c&0x000000ff)/255.0f;
    }
    COLOR operator*( float f )const{
        return( COLOR( r * f, g * f, b * f, a ) );
    }
};

//色相h、彩度S、明度V を　RGBに変換
COLOR hsv_to_rgb( float h, float s=255, float v=255 );

#endif



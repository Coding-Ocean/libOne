#include"common.h"
#include"FILE_BUFFER.h"
#include"CONTAINER/CURVE.h"

CURVE::CURVE():
    Data( 0 ),
    NumData( 0 ){
}

CURVE::~CURVE(){
    NumData = 0;
    SAFE_DELETE_ARRAY( Data );
}

void CURVE::set( FILE_BUFFER& fb ){
    fb.readString();
    WARNING( fb != "c", "cを読み込まなくてはならない", fb.string() );
    //Curve Type
    fb.readString();
    if(      fb == "rx" )
        Type = TYPE_ROTATE_X;
    else if( fb == "ry" )
        Type = TYPE_ROTATE_Y;
    else if( fb == "rz" )
        Type = TYPE_ROTATE_Z;
    else if( fb == "tx" )
        Type = TYPE_TRANSLATE_X;
    else if( fb == "ty" )
        Type = TYPE_TRANSLATE_Y;
    else if( fb == "tz" )
        Type = TYPE_TRANSLATE_Z;
    else if( fb == "sx" )
        Type = TYPE_SCALE_X;
    else if( fb == "sy" )
        Type = TYPE_SCALE_Y;
    else if( fb == "sz" )
        Type = TYPE_SCALE_Z;
    else
        WARNING( 1, "Curve Type Erorr", fb.string() );
    //NumData
    NumData = fb.readInt();
    //Data
    Data = new DATA[ NumData ];
    for( int i = 0; i < NumData; i++ ){
        fb.readString();//'d'の読み込み
        Data[ i ].frameNumber = fb.readFloat();
        Data[ i ].value = fb.readFloat();
        Data[ i ].interporationIdx = fb.readInt();
        Data[ i ].leftSlope = fb.readFloat();
        Data[ i ].rightSlope = fb.readFloat();
    }
}

CURVE::TYPE CURVE::type() const{
    return Type;
}

void  CURVE::getInterpolation( double f, float* v ) const {
    //現在のフレーム数 f を最後のデータのフレーム数で割った余りを求め f とする。
    double quot = f / Data[ NumData - 1 ].frameNumber;
    int quotInt = (int)quot;//商を整数化する
    f -= (double)quotInt * Data[ NumData - 1 ].frameNumber;

    //現在のフレーム数 f がどのデータ間にあるか探す
    int begin = 0;
    int end = 0;
    for (end = 0; end < NumData; ++end){
        if (Data[end].frameNumber > f){
            break;
        }
        begin = end;
    }

    //補完
    if (Data[begin].interporationIdx == 3){//３次補間
        if ( end >= NumData ){ //あふれ防止。最後の値返してやれ
			*v = (float)Data[ NumData - 1 ].value;
            return;
		}
        double f0 = Data[begin].frameNumber;
		double f1 = Data[ end ].frameNumber;
		double p0 = Data[ begin ].value;
		double p1 = Data[ end ].value;
		double v0 = Data[ begin ].rightSlope; //始点の右傾き
		double v1 = Data[ end ].leftSlope; //終点の左傾き
		//変数変換
		f = ( f - f0 ) / ( f1 - f0 );
		//補間
		double a = 2.0 * ( p0 - p1 ) + ( v0 + v1 );
		double b = 3.0 * ( p1 - p0 ) - ( 2.0 * v0 ) - v1;
        //af^3 + bf^2 + cf + dで計算。c=v0、d=p0だ。
		double r = a; //a
		r *= f; //af
		r += b; //af+b
		r *= f; //af^2+bf
		r += v0; //af^2+bf+c
		r *= f; //af^3+bf^2+cf
		r += p0; //af^3+bf^2+cf+d
        *v = (float)r;
    }
    else if (Data[begin].interporationIdx == 2){//線形補完
        if (end >= NumData){ //あふれ防止。最後の値返してやれ
            *v = (float)Data[NumData - 1].value;
            return;
        }
        double f0 = Data[begin].frameNumber;
        double f1 = Data[end].frameNumber;
        double v0 = Data[begin].value;
        double v1 = Data[end].value;
        //変数変換
        f = (f - f0) / (f1 - f0);
        //線形補間
        *v = float(v0 + (v1 - v0) * f);
    }
    else if (Data[begin].interporationIdx == 1){//補間なし
        *v = (float)Data[begin].value;
    }
}

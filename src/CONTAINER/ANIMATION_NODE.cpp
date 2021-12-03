#include"common.h"
#include"FILE_BUFFER.h"
#include"VECTOR.h"
#include"CONTAINER/ANIMATION_NODE.h"
#include"CONTAINER/CURVE.h"

ANIMATION_NODE::ANIMATION_NODE():
    Curves( 0 ),
    NumCurves( 0 ){
}

ANIMATION_NODE::~ANIMATION_NODE(){
    NumCurves = 0;
    SAFE_DELETE_ARRAY( Curves );
}

void ANIMATION_NODE::set( FILE_BUFFER& fb ){
    fb.readOnAssumption( "n" );
    //AnimationNode Name
    Name = fb.readString();
    //NumCurves
    NumCurves = fb.readInt();
    //Curves
    Curves = new CURVE[ NumCurves ];
    for( int i = 0; i < NumCurves; i++ ){
        Curves[ i ].set( fb );
    }    
}

const NAME& ANIMATION_NODE::name() const{
    return Name;
}

void ANIMATION_NODE::update( double frameNumber,
                             VECTOR* scale,
                             VECTOR* rotate,
                             VECTOR* translate
                           ) const {
    for ( int i = 0; i < NumCurves; ++i ){
        const CURVE& c = Curves[ i ];
        float v;
        c.getInterpolation( frameNumber, &v );
        CURVE::TYPE t = c.type();
        switch ( t ){
            case CURVE::TYPE_ROTATE_X: 
                rotate->x = v; 
                break;
            case CURVE::TYPE_ROTATE_Y: 
                rotate->y = v; 
                break;
            case CURVE::TYPE_ROTATE_Z: 
                rotate->z = v; 
                break;
            case CURVE::TYPE_TRANSLATE_X:
                translate->x = v;
                break;
            case CURVE::TYPE_TRANSLATE_Y: 
                translate->y = v; 
                break;
            case CURVE::TYPE_TRANSLATE_Z: 
                translate->z = v; 
                break;
            case CURVE::TYPE_SCALE_X: 
                scale->x = v; 
                break;
            case CURVE::TYPE_SCALE_Y: 
                scale->y = v; 
                break;
            case CURVE::TYPE_SCALE_Z: 
                scale->z = v; 
                break;
            default: 
                WARNING( true, "ありえません", "" ); 
                break;
        }
    }
}

void ANIMATION_NODE::update2( double frameNumber2,
                              double weight2,
                              VECTOR* scale,
                              VECTOR* rotate,
                              VECTOR* translate
                            ) const {
    for( int i = 0; i < NumCurves; ++i ){
        const CURVE& c = Curves[ i ];
        float v2;
        c.getInterpolation( frameNumber2, &v2 );
        CURVE::TYPE t = c.type();
        switch ( t ){
            case CURVE::TYPE_ROTATE_X: 
                rotate->x = static_cast<float>
                    ( v2 * weight2 + (double)rotate->x * ( 1.0 - weight2 ) ); 
                break;
            case CURVE::TYPE_ROTATE_Y: 
                rotate->y = static_cast<float>
                    ( v2 * weight2 + (double)rotate->y * ( 1.0 - weight2 ) );  
                break;
            case CURVE::TYPE_ROTATE_Z: 
                rotate->z = static_cast<float>
                    ( v2 * weight2 + (double)rotate->z * ( 1.0 - weight2 ) );  
                break;
            case CURVE::TYPE_TRANSLATE_X:
                translate->x = static_cast<float>
                    ( v2 * weight2 + (double)translate->x * ( 1.0 - weight2 ) );
                break;
            case CURVE::TYPE_TRANSLATE_Y: 
                translate->y = static_cast<float>
                    ( v2 * weight2 + (double)translate->y * ( 1.0 - weight2 ) ); 
                break;
            case CURVE::TYPE_TRANSLATE_Z: 
                translate->z = static_cast<float>
                    ( v2 * weight2 + (double)translate->z * ( 1.0 - weight2 ) ); 
                break;
            case CURVE::TYPE_SCALE_X: 
                scale->x = static_cast<float>
                    ( v2 * weight2 + (double)scale->x * ( 1.0 - weight2 ) ); 
                break;
            case CURVE::TYPE_SCALE_Y: 
                scale->y = static_cast<float>
                    ( v2 * weight2 + (double)scale->y * ( 1.0 - weight2 ) ); 
                break;
            case CURVE::TYPE_SCALE_Z: 
                scale->z = static_cast<float>
                    ( v2 * weight2 + (double)scale->z * ( 1.0 - weight2 ) ); 
                break;
            default: 
                WARNING( true, "ありえないカーブタイプ2", "" ); 
                break;
        }
    }
}
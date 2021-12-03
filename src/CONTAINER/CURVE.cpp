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
    WARNING( fb != "c", "c��ǂݍ��܂Ȃ��Ă͂Ȃ�Ȃ�", fb.string() );
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
        fb.readString();//'d'�̓ǂݍ���
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
    //���݂̃t���[���� f ���Ō�̃f�[�^�̃t���[�����Ŋ������]������� f �Ƃ���B
    double quot = f / Data[ NumData - 1 ].frameNumber;
    int quotInt = (int)quot;//���𐮐�������
    f -= (double)quotInt * Data[ NumData - 1 ].frameNumber;

    //���݂̃t���[���� f ���ǂ̃f�[�^�Ԃɂ��邩�T��
    int begin = 0;
    int end = 0;
    for (end = 0; end < NumData; ++end){
        if (Data[end].frameNumber > f){
            break;
        }
        begin = end;
    }

    //�⊮
    if (Data[begin].interporationIdx == 3){//�R�����
        if ( end >= NumData ){ //���ӂ�h�~�B�Ō�̒l�Ԃ��Ă��
			*v = (float)Data[ NumData - 1 ].value;
            return;
		}
        double f0 = Data[begin].frameNumber;
		double f1 = Data[ end ].frameNumber;
		double p0 = Data[ begin ].value;
		double p1 = Data[ end ].value;
		double v0 = Data[ begin ].rightSlope; //�n�_�̉E�X��
		double v1 = Data[ end ].leftSlope; //�I�_�̍��X��
		//�ϐ��ϊ�
		f = ( f - f0 ) / ( f1 - f0 );
		//���
		double a = 2.0 * ( p0 - p1 ) + ( v0 + v1 );
		double b = 3.0 * ( p1 - p0 ) - ( 2.0 * v0 ) - v1;
        //af^3 + bf^2 + cf + d�Ōv�Z�Bc=v0�Ad=p0���B
		double r = a; //a
		r *= f; //af
		r += b; //af+b
		r *= f; //af^2+bf
		r += v0; //af^2+bf+c
		r *= f; //af^3+bf^2+cf
		r += p0; //af^3+bf^2+cf+d
        *v = (float)r;
    }
    else if (Data[begin].interporationIdx == 2){//���`�⊮
        if (end >= NumData){ //���ӂ�h�~�B�Ō�̒l�Ԃ��Ă��
            *v = (float)Data[NumData - 1].value;
            return;
        }
        double f0 = Data[begin].frameNumber;
        double f1 = Data[end].frameNumber;
        double v0 = Data[begin].value;
        double v1 = Data[end].value;
        //�ϐ��ϊ�
        f = (f - f0) / (f1 - f0);
        //���`���
        *v = float(v0 + (v1 - v0) * f);
    }
    else if (Data[begin].interporationIdx == 1){//��ԂȂ�
        *v = (float)Data[begin].value;
    }
}

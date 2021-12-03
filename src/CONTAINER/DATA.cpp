#include<stdlib.h>
#include"FILE_BUFFER.h"
#include"CONTAINER/DATA.h"

DATA::DATA():
    FloatValue( 0.0f ){
}

DATA::DATA( FILE_BUFFER& fb ):
    FloatValue( 0.0f ),
    Name( fb.readString() ){
    const char* p = fb.readString();
    bool numeric = true;
    for( int i = 0; p[ i ] != 0; i++ ){
        if( '0' <= p[ i ] && p[ i ] <= '9' || p[ i ] == '-' || p[ i ] == '.' ){
        }
        else{
            numeric = false;
            break;
        }
    }
    if( numeric ){
        FloatValue = (float)atof( p );
    }
    else{
        StringValue = p;
    }
}

DATA::~DATA(){
    FloatValue = 0.0f;
}

const NAME& DATA::name() const{
    return Name;
}    


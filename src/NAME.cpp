#include<string.h>
#include"NAME.h"

NAME::NAME(){
    S = new char[ 1 ];
    S[ 0 ] = 0;
}

NAME::~NAME(){
    if( S ){ 
        delete [] S; 
        S = 0; 
    }
}

NAME::NAME( const char* s ){
    size_t len = strlen( s ) + 1;
    S = new char[ len ];
    strcpy_s( S, len, s );
}

void NAME::operator=( const char* s ){
    delete [] S;
    size_t len = strlen( s ) + 1;
    S = new char[ len ];
    strcpy_s( S, len, s );
}

int NAME::operator==( const char* s ) const {
    return( strcmp( S, s ) == 0 );
}

int NAME::operator!=( const char* s ) const {
    return( strcmp( S, s ) != 0 );
}

NAME::NAME( const NAME& n ){
    size_t len = strlen( n.S ) + 1;
    S = new char[ len ];
    strcpy_s( S, len, n.S );
}
    
void NAME::operator=( const NAME& n ){
    delete [] S;
    size_t len = strlen( n.S ) + 1;
    S = new char[ len ];
    strcpy_s( S, len, n.S );
}

int NAME::operator==( const NAME& n ) const{
    return( strcmp( S, n.S ) == 0 );
}

int NAME::operator!=( const NAME& n ) const{
    return( strcmp( S, n.S ) != 0 );
}

void NAME::operator+=( const char* s ){
    size_t l = strlen( S ) + strlen( s ) + 1;
    char* ts;//temp string
    ts = new char[ l ];
    strcpy_s( ts, l, S );
    strcat_s( ts, l, s );
    delete [] S;
    S = new char[ l ];
    strcpy_s( S, l, ts );
    delete [] ts;
}



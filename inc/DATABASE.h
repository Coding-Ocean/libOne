#pragma once
#include"DATA_ELEM.h"
class DATABASE{
public:
    DATABASE();
    DATABASE( const char* fileName );
    void operator=( const char* fileName );
    ~DATABASE();
    class DATA_ELEM& data( const char* name );
    int num() {
        return NumData;
    }
private:
    void Create( const char* fileName );
    void Destroy();
    class DATA_ELEM** Data=0;
    int NumData=0;
};

#ifndef SINGLETON_HD
#define SINGLETON_HD

#include"common.h"

template <typename T> class SINGLETON{
public:
    SINGLETON(){
        WARNING( Instance, "すでにこのインスタンスは存在します", "" );
        Instance = ( T* )this;
    }
    ~SINGLETON(){
        Instance = 0;
    }
    static T* instance(){
        return Instance;
    }
private:
    SINGLETON( const SINGLETON& );
    static T* Instance;
};

template <typename T> T* SINGLETON <T>::Instance = 0;

#endif
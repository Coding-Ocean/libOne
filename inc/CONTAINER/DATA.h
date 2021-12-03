#pragma once
#include"NAME.h"
class FILE_BUFFER;

class DATA{
public:
    DATA();
    DATA( FILE_BUFFER& fb );
    ~DATA();
    operator unsigned(){ return (unsigned)FloatValue; }
    operator int(){ return (int)FloatValue; }
    operator float(){ return FloatValue; }
    operator char*(){ return StringValue; }
    const NAME& name() const;
private:
    float FloatValue;
    NAME StringValue;
    NAME Name;
};
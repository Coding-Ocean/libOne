#pragma once
#include<string>
class DATA_ELEM{
public:
    DATA_ELEM();
    DATA_ELEM( class FILE_BUFFER& fb );
    void setName(const char* name);
    void setValue(const char* value);
    //getter
    operator unsigned(){ return (unsigned)FloatValue; }
    operator int(){ return (int)FloatValue; }
    operator float(){ return FloatValue; }
    operator const char* () { return StringValue.c_str(); }
    operator const std::string&()const{ return StringValue; }
    const std::string& name() const { return Name; }
private:
    //Value
    float FloatValue=0;
    std::string StringValue;
    //Data name
    std::string Name;
};
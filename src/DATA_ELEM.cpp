#include<string.h>
#include"FILE_BUFFER.h"
#include"DATA_ELEM.h"
DATA_ELEM::DATA_ELEM() {
}
DATA_ELEM::DATA_ELEM( FILE_BUFFER& fb ){
    setName(fb.readString());
    setValue(fb.readString());
}
void DATA_ELEM::setName(const char* name) {
    Name = name; 
}
void DATA_ELEM::setValue(const char* p) {
    //numericチェック
    bool numeric = true;
    for (int i = 0; p[i] != 0; i++) {
        if ('0' <= p[i] && p[i] <= '9' ||
            p[i] == '-' ||
            p[i] == '.') {
        }
        else {
            numeric = false;
            break;
        }
    }
    //値設定
    if (numeric) {
        FloatValue = (float)atof(p);
        StringValue = "";
    }
    else {
        StringValue = p;
        FloatValue = 0;
    }
}

#ifndef INDEX_BUFFER_HD
#define INDEX_BUFFER_HD

#include"NAME.h"
struct ID3D11Buffer;
class FILE_BUFFER;

class INDEX_BUFFER{
public:
    INDEX_BUFFER( FILE_BUFFER& fb );
    ~INDEX_BUFFER();
    ID3D11Buffer* dxObj() const;
    int num() const;
    const NAME& name() const;
private:
    ID3D11Buffer* DxObj;
    int Num;
    NAME Name;
};

#endif
#pragma once
class FILE_BUFFER{
public:
    FILE_BUFFER();
    FILE_BUFFER(const char* fileName);
    void operator= ( const char* fileName );
    ~FILE_BUFFER();
    void create(const char* fileName);
    //�o�b�t�@�|�C���^��i�߂�֐�
    const char* readString();
    float readFloat();
    int readInt();
    unsigned readUnsigned();
    void readOnAssumption( char* s );
    void skipNode();
    void restart();
    bool end();
    //�擾�n
    const char* buffer() const;
    int size();
    const char* bufferPointer() const;
    const char* string() const;
    operator const char*();
    //�I�[�o�[���[�h
    bool operator==( const char* str );
    bool operator!=( const char* str );
    bool operator==( char c );
    bool operator!=( char c );
private:
    char* Buffer=0;//�t�@�C���o�b�t�@
    int Size=0;//�o�b�t�@�T�C�Y
    const char* BufferPointer=0;//�t�@�C���o�b�t�@�̃|�C���^
    char String[ 256 ]="";//���o����������
};



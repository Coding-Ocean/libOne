#pragma once
class FILE_BUFFER{
public:
    FILE_BUFFER();
    FILE_BUFFER(const char* fileName);
    void operator= ( const char* fileName );
    ~FILE_BUFFER();
    void create(const char* fileName);
    //バッファポインタを進める関数
    const char* readString();
    float readFloat();
    int readInt();
    unsigned readUnsigned();
    void readOnAssumption( char* s );
    void skipNode();
    void restart();
    bool end();
    //取得系
    const char* buffer() const;
    int size();
    const char* bufferPointer() const;
    const char* string() const;
    operator const char*();
    //オーバーロード
    bool operator==( const char* str );
    bool operator!=( const char* str );
    bool operator==( char c );
    bool operator!=( char c );
private:
    char* Buffer=0;//ファイルバッファ
    int Size=0;//バッファサイズ
    const char* BufferPointer=0;//ファイルバッファのポインタ
    char String[ 256 ]="";//取り出した文字列
};



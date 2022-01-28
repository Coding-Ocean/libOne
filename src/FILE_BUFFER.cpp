#include"common.h"
#include"FILE_BUFFER.h"
FILE_BUFFER::FILE_BUFFER(){
}
FILE_BUFFER::FILE_BUFFER(const char* fileName) {
    create(fileName);
}
void FILE_BUFFER::operator= (const char* fileName) {
    create(fileName);
}
FILE_BUFFER::~FILE_BUFFER() {
    SAFE_DELETE_ARRAY(Buffer);
}
void FILE_BUFFER::create(const char* fileName) {
    SAFE_DELETE_ARRAY(Buffer);
    FILE* fp = 0;
    fopen_s(&fp, fileName, "rb");
    WARNING(!fp, "file open error", fileName);
    //ファイルサイズを調べる
    fseek(fp, 0L, SEEK_END);//ファイルポインタをファイルの最後まで進める
    Size = ftell(fp);//ファイルサイズを取得
    rewind(fp);//ファイルポインタを最初に戻す
    //バッファを確保して一気に読み込む
    Buffer = new char[Size];
    fread(Buffer, 1, Size, fp);
    fclose(fp);
    //先頭アドレスを入れておく
    BufferPointer = Buffer;
    String[0] = 0;
}
bool FILE_BUFFER::end(){
    return ( BufferPointer - Buffer >= Size );
}
const char* FILE_BUFFER::readString(){
    //変数名を短くする
    const char*& bp = BufferPointer;
    //最後まで進んだので終了
    if( bp - Buffer >= Size ){
        return 0;
    }
    //区切り文字を読み飛ばす
    while ( bp - Buffer < Size && strchr( " =\t\r\n,;\"/", *bp ) ){
        //区切り文字を読み飛ばす
        while( bp - Buffer < Size && strchr( " =\t\r\n,;\"", *bp ) ){
            bp++;
        }
        //コメント読み飛ばし
        if ( *bp == '/' ){
            if ( *( bp + 1 ) == '/' ){
                while ( bp - Buffer < Size && !strchr( "\r\n", *bp ) ){
                    bp++;
                }
            }
            else{
                break;
            }
        }
    }
    //文字列をStringへ取り出す
    char* sp = String;
    while ( bp - Buffer < Size && !strchr( " =\t\r\n,;\"", *bp ) ){
        //行中コメントならbreak
        if ( *bp == '/' && *(bp + 1) == '/'){
            break;
        }
        else{
            *sp++ = *bp++;
        }
    }
    *sp = 0;
    if( String[ 0 ] == 0 ){
        return 0;
    }
    return String;
}
void FILE_BUFFER::readOnAssumption( const char* s ){
    readString();
    if( strcmp( s, String ) != 0 ){
        char tmp1[256]="読むと想定した文字列:";
        char tmp2[256]="実際に、読んだ文字列:";
        strcat_s( tmp1, 256, s );
        strcat_s( tmp2, 256, String );
        WARNING( 1, tmp1, tmp2 );
    }
}
float FILE_BUFFER::readFloat(){
    readString();
    return ( float ) atof( String );
}
int FILE_BUFFER::readInt(){
    readString();
    return atoi( String );
}
unsigned FILE_BUFFER::readUnsigned(){
    readString();
    return ( unsigned ) atoi( String );
}
void FILE_BUFFER::skipNode(){
    int cnt = 1;
    while( cnt > 0 ){
        readString();
        if( String[0] == '{' ){
            cnt++;
        }
        if( String[0] == '}' ){
            cnt--;
        }
    }
}
void FILE_BUFFER::restart(){
    BufferPointer = Buffer;
}
const char* FILE_BUFFER::buffer() const{
    return Buffer;
}
int FILE_BUFFER::size(){
    return Size;
}
const char* FILE_BUFFER::string() const{
    return String;
}
const char* FILE_BUFFER::bufferPointer() const{
    return BufferPointer;
}
bool FILE_BUFFER::operator==( const char* str ){
    return strcmp( String, str ) == 0;
}
bool FILE_BUFFER::operator!=( const char* str ){
    return strcmp( String, str ) != 0;
}
bool FILE_BUFFER::operator==( char c ){
    return ( String[0] == c );
}
bool FILE_BUFFER::operator!=( char c ){
    return ( String[0] != c );
}
FILE_BUFFER::operator const char *(){
    return String;
}
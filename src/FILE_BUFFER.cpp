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
    //�t�@�C���T�C�Y�𒲂ׂ�
    fseek(fp, 0L, SEEK_END);//�t�@�C���|�C���^���t�@�C���̍Ō�܂Ői�߂�
    Size = ftell(fp);//�t�@�C���T�C�Y���擾
    rewind(fp);//�t�@�C���|�C���^���ŏ��ɖ߂�
    //�o�b�t�@���m�ۂ��Ĉ�C�ɓǂݍ���
    Buffer = new char[Size];
    fread(Buffer, 1, Size, fp);
    fclose(fp);
    //�擪�A�h���X�����Ă���
    BufferPointer = Buffer;
    String[0] = 0;
}
bool FILE_BUFFER::end(){
    return ( BufferPointer - Buffer >= Size );
}
const char* FILE_BUFFER::readString(){
    //�ϐ�����Z������
    const char*& bp = BufferPointer;
    //�Ō�܂Ői�񂾂̂ŏI��
    if( bp - Buffer >= Size ){
        return 0;
    }
    //��؂蕶����ǂݔ�΂�
    while ( bp - Buffer < Size && strchr( " =\t\r\n,;\"/", *bp ) ){
        //��؂蕶����ǂݔ�΂�
        while( bp - Buffer < Size && strchr( " =\t\r\n,;\"", *bp ) ){
            bp++;
        }
        //�R�����g�ǂݔ�΂�
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
    //�������String�֎��o��
    char* sp = String;
    while ( bp - Buffer < Size && !strchr( " =\t\r\n,;\"", *bp ) ){
        //�s���R�����g�Ȃ�break
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
        char tmp1[256]="�ǂނƑz�肵��������:";
        char tmp2[256]="���ۂɁA�ǂ񂾕�����:";
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
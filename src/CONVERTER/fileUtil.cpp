#include<stdio.h>
#include<string.h>
#include"common.h"

void addFile( const char* fileName, const char* addFileName ){
    FILE* fp;

    //input
    fp = 0;
    fopen_s( &fp, addFileName, "rb");
    WARNING( !fp, "file open error", addFileName );
    fseek( fp, 0L, SEEK_END );
	unsigned Size = ftell( fp );//�t�@�C���T�C�Y���擾
    rewind( fp );//�t�@�C���|�C���^���ŏ��ɖ߂�
    char* Buffer = new char[ Size ];//�o�b�t�@���m�ۂ���
    fread( Buffer, 1, Size, fp );//��C�ɓǂݍ���
    fclose( fp );

	//output
    fp = 0;
    fopen_s( &fp, fileName, "ab");
	if( !fp ){
		delete [] Buffer;
		WARNING( 1, "file open error", fileName );
	}
	fwrite( Buffer, 1, Size, fp );
    fclose( fp );
	SAFE_DELETE_ARRAY( Buffer );
}

void delFile( const char* fileName ){
    FILE* fp = 0;
    fopen_s( &fp, fileName, "wb");
	fclose( fp );
}
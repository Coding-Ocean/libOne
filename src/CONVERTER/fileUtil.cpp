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
	unsigned Size = ftell( fp );//ファイルサイズを取得
    rewind( fp );//ファイルポインタを最初に戻す
    char* Buffer = new char[ Size ];//バッファを確保して
    fread( Buffer, 1, Size, fp );//一気に読み込む
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
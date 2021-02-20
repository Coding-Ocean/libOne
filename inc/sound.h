#pragma once

//初期化開放系
void initSound();
void freeSound();//全ての音を開放
void releaseSound( int soundIdx );//ひとつの音だけ開放

//ロード
int loadSound( const char* filename );
int duplicateSound( int i );

//出力系
void playSound( int soundIdx );
void playLoopSound( int soundIdx );//繰り返し再生
void stopSound( int soundIdx );
void setVolume( int soundIdx, long volume );
void setPan( int soundIdx, int pan );//panの値域は-10000～10000

//テスト
//init();
//free();
//void play( char* fileName );
//void autoReleaseSound();


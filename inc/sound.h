#pragma once

//�������J���n
void initSound();
void freeSound();//�S�Ẳ�����уT�E���h�V�X�e�����J��
void releaseSound(int soundIdx);//�ЂƂ̉������J��
void releaseAllSound();//�S�Ẳ����J��

//���[�h
int loadSound( const char* filename );
int duplicateSound( int soundIdx );

//�o�͌n
void playSound( int soundIdx );
void playLoopSound( int soundIdx );//�J��Ԃ��Đ�
void stopSound( int soundIdx );
void setVolume( int soundIdx, long volume );
void setPan( int soundIdx, int pan );//pan�̒l���-10000�`10000

//�e�X�g
//init();
//free();
//void play( char* fileName );
//void autoReleaseSound();


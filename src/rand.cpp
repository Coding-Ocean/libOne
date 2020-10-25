#include <windows.h>

unsigned X;
unsigned Y;
unsigned Z;
unsigned W;

void setRandSeed(){
	X = 123456789;
	Y = 362436069;
	Z = 521288629;
	W = timeGetTime();
}
void setRandSeed( int seed ){
	X = 123456789;
	Y = 362436069;
	Z = 521288629;
	W = ( seed <= 0 ) ? 88675123 : seed;
}

unsigned getRand(){
	unsigned t = X ^ ( X << 11 );
	X = Y;
	Y = Z;
	Z = W;
	W = ( W ^ ( W >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
	return W;
}
int getRandInt(){
	return static_cast< int >( getRand() );
}
int getRandInt( int a ){
	int x = getRandInt() & 0x7fffffff; //�����r�b�g�؎̂āB���x�͗����邪�������Ȃ��Ɣ͈͂ɓ���Ȃ�
	x %= a + 1;
	return x;
}
int getRandInt( int a, int b ){
	int x = getRandInt() & 0x7fffffff; //�����r�b�g�؎̂āB���x�͗����邪�������Ȃ��Ɣ͈͂ɓ���Ȃ�
	x %= b + 1 - a;
	x += a;
	return x;
}
float getRandFloat(){
	int x = getRandInt();
	x = ( x & 0x007fffff ) | 0x3f800000; //�w�����ɌŒ��127���Ԃ�����
	float f = *reinterpret_cast< float* >( &x ); //�������L���X�g
	return f - 1.f; //��̍s��1-2�ɂȂ��Ă���
}
float getRandFloat( float a ){
	float f = getRandFloat();
	f *= a;
	return f;
}
float getRandFloat( float a, float b ){
	float f = getRandFloat();
	f *= b - a;
	f += a;
	return f;
}


unsigned random() {
	unsigned t = X ^ (X << 11);
	X = Y;
	Y = Z;
	Z = W;
	W = (W ^ (W >> 19)) ^ (t ^ (t >> 8));
	return W;
}
int random(int a) {
	int x = getRandInt() & 0x7fffffff; //�����r�b�g�؎̂āB���x�͗����邪�������Ȃ��Ɣ͈͂ɓ���Ȃ�
	x %= a + 1;
	return x;
}
int random(int a, int b) {
	int x = getRandInt() & 0x7fffffff; //�����r�b�g�؎̂āB���x�͗����邪�������Ȃ��Ɣ͈͂ɓ���Ȃ�
	x %= b + 1 - a;
	x += a;
	return x;
}
float random(float a) {
	float f = getRandFloat();
	f *= a;
	return f;
}
float random(float a, float b) {
	float f = getRandFloat();
	f *= b - a;
	f += a;
	return f;
}

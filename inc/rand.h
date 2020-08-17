#ifndef Rand_HD
#define Rand_HD

void setRandSeed();
unsigned getRand();

void setRandSeed( int seed );
int getRandInt();
int getRandInt( int a );
int getRandInt( int a, int b );
float getRandFloat();
float getRandFloat( float a );
float getRandFloat( float a, float b );


#endif
#pragma once
void createPackage(const char* directry, const char* filename);

void loadPackage(const char* filename);
void deletePackage();

//for loadImage function in graphi.cpp
#pragma once
bool packageDataExists();
unsigned char* getPackageData(const char* filename, int* size);

#pragma once
void createPackage(const char* directry, const char* filename);

void loadPackage(const char* filename);
void deletePackage();

//for loadImage function in graphi.cpp
unsigned char* getData(const char* filename, int* size);
const unsigned char* packageData();

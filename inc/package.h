#pragma once
//Create--------------------------------------------------------
void setNumFiles(int numFiles);
void setFilename(int i, const char* filename);
void createPackage(const char* filename);

//Load----------------------------------------------------------
void loadPackage(const char* filename);
void deletePackage();

unsigned char* getData(const char* filename, int* size);


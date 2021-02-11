#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<memory.h>
#include<process.h>
#include <sys\stat.h>
#include"package.h"

struct INDEX {
    char filename[32]="";
    int startIdx = 0;
    int size = 0;
};

int NumFiles = 0;
struct INDEX* FileIndices = 0;
unsigned int SizePackage = 0;
unsigned char* PackageData = 0;

void loadPackage(const char* filename) {
    assert(PackageData == 0);
    FILE* fp;
    //インデックス読み込み
    fopen_s(&fp, filename, "rb");
    fread(&NumFiles, sizeof(int), 1, fp);
    FileIndices = new INDEX[NumFiles];
    fread(FileIndices, sizeof(INDEX), NumFiles, fp);
    //パッケージデータ読み込み
    fread(&SizePackage, sizeof(unsigned int), 1, fp);
    PackageData = new unsigned char[SizePackage];
    fread(PackageData, SizePackage, 1, fp);
    fclose(fp);
}
void deletePackage() {
    delete[]PackageData;
    PackageData = 0;
    SizePackage = 0;
    delete[] FileIndices;
    FileIndices = 0;
    NumFiles = 0;
}

unsigned char* getData(const char* filename, int* size) {
    int i = 0;
    while (i < NumFiles) {
        if (strcmp(FileIndices[i].filename, filename) == 0) {
            break;
        }
        i++;
    }
    assert(i < NumFiles);

    //Dataからデータを取り出す
    assert(PackageData != 0);
    *size = FileIndices[i].size;
    return PackageData + FileIndices[i].startIdx;
}

//Create--------------------------------------------------------
void setNumFiles(int numFiles) {
    NumFiles = numFiles;
    FileIndices = new INDEX[numFiles]();
}
void setFilename(int i, const char* filename) {
    strcpy_s(FileIndices[i].filename, filename);
}
void createIndices(const char* filename) {
    FILE* fp;

    SizePackage = 0;
    for (int i = 0; i < NumFiles; i++) {
        fopen_s(&fp, FileIndices[i].filename, "rb");
        assert(fp != 0);
        struct stat info;
        fstat(_fileno(fp), &info);
        FileIndices[i].size = info.st_size;
        if (i > 0) {
            FileIndices[i].startIdx = FileIndices[i - 1].startIdx + FileIndices[i - 1].size;
        }
        SizePackage += FileIndices[i].size;
        fclose(fp);
    }
    //パッケージファイルの先頭にインデックスを出力する
    fopen_s(&fp, filename, "wb");
    fwrite(&NumFiles, sizeof(int), 1, fp);
    fwrite(FileIndices, sizeof(INDEX), NumFiles, fp);
    fwrite(&SizePackage, sizeof(unsigned int), 1, fp);
    fclose(fp);
}
void addFile(const char* filename) {
    FILE* fp;
    for (int i = 0; i < NumFiles; i++) {

        fopen_s(&fp, FileIndices[i].filename, "rb");
        char* data = new char[FileIndices[i].size];
        fread(data, FileIndices[i].size, 1, fp);
        fclose(fp);

        fopen_s(&fp, filename, "ab");
        fwrite(data, FileIndices[i].size, 1, fp);
        fclose(fp);
        delete[] data;
    }
}
void createPackage(const char* filename) {
    createIndices(filename);
    addFile(filename);
    deletePackage();
}



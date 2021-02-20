#include<fstream>
#include<filesystem>
#include"common.h"
#include"package.h"

struct INDEX {
    char filename[32]="";
    int startIdx = 0;
    int size = 0;
};

int NumFiles = 0;
struct INDEX* FileIndices = 0;
int SizePackage = 0;
unsigned char* PackageData = 0;

bool packageDataExists() {
    return PackageData != 0;
}
void loadPackage(const char* filename) {
    //assert(PackageData == 0);
    FILE* fp;
    //インデックス読み込み
    fopen_s(&fp, filename, "rb");
    WARNING(fp == 0, "パッケージファイルオープンエラー", filename);
    fread(&NumFiles, sizeof(int), 1, fp);
    FileIndices = new INDEX[NumFiles];
    fread(FileIndices, sizeof(INDEX), NumFiles, fp);
    //パッケージデータ読み込み
    fread(&SizePackage, sizeof(int), 1, fp);
    PackageData = new unsigned char[SizePackage];
    fread(PackageData, SizePackage, 1, fp);
    fclose(fp);
}
void deletePackage() {
    if (PackageData) {
        delete[]PackageData;
        PackageData = 0;
        SizePackage = 0;
    }
    if (FileIndices) {
        delete[] FileIndices;
        FileIndices = 0;
        NumFiles = 0;
    }
}

unsigned char* getPackageData(const char* filename, int* size) {
    int i = 0;
    while (i < NumFiles) {
        if (strcmp(FileIndices[i].filename, filename) == 0) {
            break;
        }
        i++;
    }
    WARNING(i >= NumFiles,"パッケージデータ取得エラー",filename);

    *size = FileIndices[i].size;
    return PackageData + FileIndices[i].startIdx;
}

//Create--------------------------------------------------------
void countFiles(std::string path) {
    for (const std::filesystem::directory_entry& e 
        : std::filesystem::directory_iterator(path)) {
        if (is_directory(e)) {
            countFiles(e.path().string());
        }
        else {
            NumFiles++;
        }
    }
}
void setFilename(int i, const char* filename) {
    strcpy_s(FileIndices[i].filename, filename);
}
void setNameToIdx(int* i, std::string path) {
    for (const std::filesystem::directory_entry& e 
        : std::filesystem::directory_iterator(path)) {
        if (is_directory(e)) {
            setNameToIdx(i,e.path().string());
        }
        else {
            setFilename(*i, e.path().string().c_str());
            *i += 1;
        }
    }
}
void listsFile(const char* directory) {
    NumFiles = 0;
    countFiles(directory);
    FileIndices = new INDEX[NumFiles]();
    int i = 0;
    setNameToIdx(&i,directory);
}
void createFileIndices(const char* filename) {
    //パッケージデータのインデックスを作る
    SizePackage = 0;
    for (int i = 0; i < NumFiles; i++) {
        FileIndices[i].size = (int)std::filesystem::file_size(FileIndices[i].filename);
        if (i > 0) {
            FileIndices[i].startIdx = FileIndices[i - 1].startIdx + FileIndices[i - 1].size;
        }
        SizePackage += FileIndices[i].size;
    }
    //パッケージファイルの先頭にインデックスを出力する
    FILE* fp;
    fopen_s(&fp, filename, "wb");
    fwrite(&NumFiles, sizeof(int), 1, fp);
    fwrite(FileIndices, sizeof(INDEX), NumFiles, fp);
    fwrite(&SizePackage, sizeof(int), 1, fp);
    fclose(fp);
}
#if 1
void addPackageFile(const char* filename) {
    char buf[1024];
    for (int i = 0; i < NumFiles; i++) {
        //system関数でパッケージファイルに追加出力
        sprintf_s(buf, "type %s >> %s", FileIndices[i].filename, filename);
        system(buf);
    }
}
#else
void addPackageFile(const char* filename) {
    FILE* fp;
    for (int i = 0; i < NumFiles; i++) {
        //インデックスに登録されたファイルをメモリに読み込む
        fopen_s(&fp, FileIndices[i].filename, "rb");
        char* data = new char[FileIndices[i].size];
        fread(data, FileIndices[i].size, 1, fp);
        fclose(fp);
        //パッケージファイルに追加出力
        fopen_s(&fp, filename, "ab");
        fwrite(data, FileIndices[i].size, 1, fp);
        fclose(fp);
        delete[] data;
    }
}
#endif
void createPackage(const char* directry, const char* filename) {
    listsFile(directry);
    createFileIndices(filename);
    addPackageFile(filename);
    deletePackage();
}



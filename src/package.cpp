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
    //�C���f�b�N�X�ǂݍ���
    fopen_s(&fp, filename, "rb");
    WARNING(fp == 0, "�p�b�P�[�W�t�@�C���I�[�v���G���[", filename);
    fread(&NumFiles, sizeof(int), 1, fp);
    FileIndices = new INDEX[NumFiles];
    fread(FileIndices, sizeof(INDEX), NumFiles, fp);
    //�p�b�P�[�W�f�[�^�ǂݍ���
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
    WARNING(i >= NumFiles,"�p�b�P�[�W�f�[�^�擾�G���[",filename);

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
    //�p�b�P�[�W�f�[�^�̃C���f�b�N�X�����
    SizePackage = 0;
    for (int i = 0; i < NumFiles; i++) {
        FileIndices[i].size = (int)std::filesystem::file_size(FileIndices[i].filename);
        if (i > 0) {
            FileIndices[i].startIdx = FileIndices[i - 1].startIdx + FileIndices[i - 1].size;
        }
        SizePackage += FileIndices[i].size;
    }
    //�p�b�P�[�W�t�@�C���̐擪�ɃC���f�b�N�X���o�͂���
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
        //system�֐��Ńp�b�P�[�W�t�@�C���ɒǉ��o��
        sprintf_s(buf, "type %s >> %s", FileIndices[i].filename, filename);
        system(buf);
    }
}
#else
void addPackageFile(const char* filename) {
    FILE* fp;
    for (int i = 0; i < NumFiles; i++) {
        //�C���f�b�N�X�ɓo�^���ꂽ�t�@�C�����������ɓǂݍ���
        fopen_s(&fp, FileIndices[i].filename, "rb");
        char* data = new char[FileIndices[i].size];
        fread(data, FileIndices[i].size, 1, fp);
        fclose(fp);
        //�p�b�P�[�W�t�@�C���ɒǉ��o��
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



#if( _DEBUG )

#pragma warning( disable : 4996 )
#include<string>
#include"CONVERTER\\convertUtil.h"

//string utillity
//––”ö‚Ì0‚Ío—Í‚µ‚È‚¢
char* nonZero(char* s, float f){
    sprintf(s, "%f", f);
    size_t len = strlen(s);
    for (size_t l = len - 1; l > 0; l--){
        if (s[l] == '0'){
            s[l] = '\0';
        }
        else if (s[l] == '.'){
            s[l] = '\0';
            break;
        }
        else {
            break;
        }
    }
    return s;
}

void fprintNonZero(FILE* fp, float f){
    char s[64];
    sprintf(s, "%f", f);
    size_t len = strlen(s);
    for (size_t l = len - 1; l > 0; l--){
        if (s[l] == '0'){
            s[l] = '\0';
        }
        else if (s[l] == '.'){
            s[l] = '\0';
            break;
        }
        else {
            break;
        }
    }
    fprintf(fp, "%s ", s);
}

#endif
#include <string.h>

int isLetter(char c){
    if(c >=65 && c <= 90){
        return 1 ;
    }else if(c >= 97 && c <= 122){
        return -1 ;
    }
    return 0 ;
}

char upperToLower(char c){
    return c + 32 ;
}

char lowerToUpper(char c){
    return c - 32 ;
}

char encodeChar(char c){
    int charType = isLetter(c) ;
    if(charType == -1){
        return lowerToUpper(c) ;
    }else if(charType == 1){
        return upperToLower(c) ;
    }else{
        return c ;
    }
}

void encode(char* str){
    int length = strlen(str) ;
    for(int i = 0 ; i < length ; i++){
        str[i] = encodeChar(str[i]) ;
    }
}

void decode(char* str){
    encode(str) ;
}

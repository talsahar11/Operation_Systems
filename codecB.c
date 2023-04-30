#include <string.h>
#include <stdio.h>
char promote(char c){
    if(c >= 125){
        int difference = c - 125 ;
        return -128 + difference ;
    }else{
        return c+3 ;
    }
}
char demote(char c){
    if(c <= -126){
        int difference = 126 -(c) ;
        return 128 - difference ;
    }else{
        return c-3 ;
    }
}
void encode(char* str){
    int length = strlen(str) ;
    for(int i = 0 ; i < length ; i++){
        str[i] = promote(str[i]) ;
    }
}

void decode(char* str){
    int length = strlen(str) ;
    for(int i = 0 ; i < length ; i++) {
        str[i] = demote(str[i]) ;
    }
}
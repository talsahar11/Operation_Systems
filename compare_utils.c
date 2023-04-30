#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 8192

///-----Check if a given byte is a letter. returns 1 if its true and 0 otherwise-----
int is_letter(char c){
    if((c > 64 && c < 91) || (c > 96 && c < 123)){
        return 1 ;
    }
    return 0 ;
}

///-----Gets two letters and check if one is an upper-case of the other one. returns 1 if its true, and 0 otherwise-----
int is_letters_equal(char c1, char c2){
    if(c1 > 64 && c1 < 91){
        if(c2 == c1 +32){
            return 1 ;
        }
    }else{
        if(c1 == c2 + 32){
            return 1 ;
        }
    }
    return 0 ;
}

///-----Compare two bytes, returns 0 if not equals and 1 if is equal, the method considers the (is_case_sensitive)-----
///-----flag and if the given bytes are letters, they are being compared by the value of it-----
int cmp_bytes(char c1, char c2, int is_case_sensitive){

    //-----If the two bytes holds different values, check if the is_case_sensitive flag is off,-----
    //-----and both bytes represents letters, ignore case-----
    if(c1 != c2){
        if(!is_case_sensitive && is_letter(c1) && is_letter(c2)) {
            if (!is_letters_equal(c1, c2)) {
                return 0;
            }
            return 1;
        }else{
            return 0 ;
        }
    }
    return 1 ;
}

///-----Compare two buffers byte after byte, the buffers must be at the same size, the size which provided as a-----
///-----param. The method returns 0 if the buffers are equal, and 1 if not-----
int cmp_buffers(const char* buff1, const char* buff2, ulong buffSize, int is_case_sensitive) {
    char c1 = 0, c2 = 0 ;

    //-----For each two bytes of the two buffers, compare them to each other-----
    for(int i = 0 ; i < buffSize ; i++){
        c1 = buff1[i] ;
        c2 = buff2[i] ;
        if(!cmp_bytes(c1, c2, is_case_sensitive)){
            return 1;
        }
    }
    return 0 ;
}

///-----Compare two files, return 0 if equal, and 1 if not equal-----
int cmp(FILE* f1, FILE* f2, int is_case_sensitive){

    //-----Firstly check that the both files holds the same length, if not, return 1-----
    fseek(f1, 0, SEEK_END) ;
    fseek(f2, 0, SEEK_END) ;
    ulong len1 = ftell(f1) ;
    ulong len2 = ftell(f2) ;
    if(len1 != len2){
        return 1 ;
    }

    //-----Set the files cursor to the start and create a buffers-----
    fseek(f1, 0, SEEK_SET) ;
    fseek(f2, 0, SEEK_SET) ;
    char buff1[CHUNK_SIZE], buff2[CHUNK_SIZE] ;

    ulong res1 = 0, res2 = 0 ;

    //-----While files have not been fully read, keep reading the data into the buffers and the compare the buffers-----
    while(!feof(f1) && !feof(f2)){

        res1 = fread(buff1, 1, 8192, f1);
        res2 = fread(buff2, 1, 8192, f2);

        //-----If the number of elements that been read into the both buffers is different return 1(not equal)-----
        if(res1 != res2){
            return 1 ;
        }else{
            if(cmp_buffers(buff1, buff2, res1, is_case_sensitive)){
                return 1 ;
            }
        }
    }
    return 0 ;
}
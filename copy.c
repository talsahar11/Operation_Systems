#include "compare_utils.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHUNK_SIZE 8192

///-----Flags-----
int is_force_on = 0 ;
int is_verbose_on = 0 ;
char* res_msg = NULL ;
///-----Validate that the num of args supplied is > 2, at least two file paths need to be provided. If not,-----
/// -----exit with an error message-----
void validateArgs(int argc){
    if(argc < 3){
        printf("There are at least 2 extra arguments needs to be provided. The current provided args count is: %d.\n"
               "The copy usage is as follows: ./copy <source-file> <destination-file> \navailable options:\n"
               "    -f -> force override an existing file.\n    -v -> verbose output.\n"
               " exit.\n", argc-1) ;
        exit(1) ;
    }
}

///-----Read the additional options from the argv array if they exists, and set the flags to the wanted value-----
void set_options(char** argv, int argc){
    char* option = NULL ;
    if(argc > 3){
        for(int i = 3 ; i < argc ; i++){
            option = argv[i] ;
            if(strcmp(option, "-v") == 0){
                is_verbose_on = 1 ;
            }
            else if(strcmp(option, "-f") == 0){
                is_force_on = 1 ;
            }
            else{
                printf("%s is not an option.\nThe available options are:\n    -v -> verbose output.\n"
                       "    -f -> force override an existing file.\n", argv[i]) ;
                exit(1) ;
            }
        }
    }
}

int is_file_exist(char* file_path){
    FILE *f = fopen(file_path, "r") ;
    if(f == NULL){
        return 0 ;
    }
    return 1 ;
}

int open_files(char** argv, FILE** src_f, FILE** dest_f){
    *src_f = fopen(argv[1], "r") ;
    if(is_file_exist(argv[2])){
        if(!is_force_on){
            res_msg = "target file exists" ;
            return 0 ;
        }else{
            *dest_f = fopen(argv[2], "w") ;
            fclose(*dest_f) ;
        }
    }
    *dest_f = fopen(argv[2], "a+") ;
    if(*src_f == NULL || *dest_f == NULL){
        res_msg = "general failure" ;
        return 0 ;
    }
    return 1 ;
}

int copy(FILE* src_f, FILE* dest_f){
    char* buffer[CHUNK_SIZE] ;
    ulong current_len = -1 ;
    fseek(src_f, 0, SEEK_SET) ;
    while(!feof(src_f)){
        current_len = fread(buffer, 1, CHUNK_SIZE, src_f) ;
        if(fwrite(buffer, 1, current_len, dest_f) != current_len){
            res_msg = "general failure" ;
            fclose(src_f) ;
            fclose(dest_f) ;
            return 0 ;
        }
    }
    return 1 ;
}

void print_if_needed(){
    if(is_verbose_on){
        printf("%s\n", res_msg) ;
    }
}
int main(int argc, char** argv){
    validateArgs(argc) ;
    set_options(argv, argc) ;
    FILE *src_f = NULL, *dest_f = NULL ;
    if(!open_files(argv, &src_f, &dest_f)){
        print_if_needed() ;
        return 1 ;
    }
    if(!copy(src_f, dest_f)){
        print_if_needed() ;
        return 1 ;
    }
    if(cmp(src_f, dest_f, 0) != 0){
        res_msg = "general failure" ;
        print_if_needed() ;
        fclose(src_f) ;
        fclose(dest_f) ;
        return 1 ;
    }
    fclose(src_f) ;
    fclose(dest_f) ;
    res_msg = "success" ;
    print_if_needed() ;
    return 0 ;
}
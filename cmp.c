#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compare_utils.c"

///----Flags-----
int is_verbose_on = 0 ;                                 //-----False by default-----
int is_case_sensitive = 1;                              //-----True by default-----

///-----Validate that the num of args supplied is > 2, at least two file paths need to be provided. If not,-----
/// -----exit with an error message-----
void validateArgs(int argc){
    if(argc < 3){
        printf("There are at least 2 extra arguments needs to be provided. The current provided args count is: %d.\n"
               "The cmp usage is as follows: ./cmp <file1> <file2> \navailable options:\n"
               "    -i -> ignore case.\n    -v -> verbose output.\n exit.\n", argc-1) ;
        exit(-1) ;
    }
}

///-----Open two files supplied as an arguments to the program, if there is no such files, exit with an error message-----
void open_files(FILE **f1, FILE **f2, char* name1, char* name2){
    *f1 = fopen(name1, "r") ;
    *f2 = fopen(name2, "r") ;
    if(*f1 == NULL || *f2 == NULL){
        char* f_name = (*f1 == NULL) ? name1 : name2 ;
        printf("The file: %s has not found in the current lib.\n", f_name) ;
        exit(-1) ;
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
            else if(strcmp(option, "-i") == 0){
                is_case_sensitive = 0 ;
            }
            else{
                printf("%s is not an option.\nThe available options are:\n    -v -> verbose output.\n"
                       "    -i -> ignore case.\n", argv[i]) ;
            }
        }
    }
}


int main(int argc, char** argv) {
    validateArgs(argc) ;                                     //-----Validate that the min num of args(2) is received---
    FILE *f1 = NULL, *f2 = NULL ;
    open_files(&f1, &f2, argv[1], argv[2]) ;   //-----Open the files, exit if one of them is not exists---
    set_options(argv, argc) ;                               //-----Setting the options, if there are any-----
    int res = cmp(f1, f2, is_case_sensitive) ;                                 //-----Compare the files-----
    if(is_verbose_on){                                      //-----Check if printing the result is needed-----
        if(res == 0){
            printf("equal\n") ;
        }else{
            printf("distinct\n") ;
        }
    }
    fclose(f1) ;
    fclose(f2) ;
    return res ;
}

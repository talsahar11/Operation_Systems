#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char* argv[]){
    void* lib_handle ;
    if(strcmp(argv[1], "codecA") == 0){
        lib_handle = dlopen("libcodecA.so", RTLD_LAZY);
        if (lib_handle == NULL) {
            fprintf(stderr, "Error loading library: %s\n", dlerror());
            return -1 ;
        }
    }else if(strcmp(argv[1], "codecB") == 0){
        lib_handle = dlopen("libcodecB.so", RTLD_LAZY);
        if (lib_handle == NULL) {
            fprintf(stderr, "Error loading library: %s\n", dlerror());
            return -1 ;
        }
    }else{
        printf("Unsupported codec provided, exit.\n") ;
        return -1 ;
    }
    typedef void (*codec_func)(char*);
    codec_func encode = (codec_func) dlsym(lib_handle, "encode");
    if (encode == NULL) {
        fprintf(stderr, "Error looking up symbol: %s\n", dlerror());
        return -1 ;
    }
    encode(argv[2]) ;
    printf("%s\n", argv[2]) ;
    return 0 ;
}
#include <sys/wait.h>
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

void ignore_sigint(int sig) {
    printf("Received SIGINT, stopping tool...\ncommand: ");
    fflush(stdout);
}

void handle_sigint(int sig) {
    kill(0, SIGTERM);  // terminate all child processes
    fflush(stdout);
}

void split_args(char* command, char* args_lists[4][10], int* lists_num){
    char *token, *word ;
    char *non_tok_args[4] ;
    int i = 0 ;
    token = strtok(command, ">|") ;
    while(token != NULL){
        non_tok_args[i] = token ;
        i++ ;
        token = strtok(NULL, ">|") ;
    }
    for(int j = 0 ; j < i ; j++){
        word = strtok(non_tok_args[j], " ") ;
        int k = 0 ;
        while(word != NULL){
            args_lists[j][k] = word ;
            word = strtok(NULL, " ") ;
            k++ ;
        }
        args_lists[j][k] = NULL ;
    }
    *lists_num = i ;
}
void set_ops_arr(char* command, char* ops_arr[3], int* ops_num){
    int cmd_len = strlen(command) ;
    int i = 0, j = 0 ;
    char current ;
    while(j < cmd_len){
        current = command[j] ;
        if(current == '>'){
            if(command[j+1] == '>'){
                ops_arr[i] = ">>" ;
                i++ ;
                j++ ;
            }else{
                ops_arr[i] = ">" ;
                i++ ;
            }
        }else if(current == '|'){
            ops_arr[i] = "|" ;
            i++ ;
        }
        j++ ;
    }
    *ops_num = i ;
}

void handle_pipe(char* args_lists[4][10], char* ops_arr[3], int ops_num) {
    if (strcmp(ops_arr[ops_num-1], ">") == 0) {
        FILE *file = fopen(args_lists[ops_num][0], "w");
        int fd = fileno(file);
        dup2(fd, STDOUT_FILENO);
//        fclose(file);
    } else if (strcmp(ops_arr[ops_num-1], ">>") == 0) {
        FILE *file = fopen(args_lists[ops_num][0], "a");
        int fd = fileno(file);
        dup2(fd, STDOUT_FILENO);
//        fclose(file);
    }
    if (strcmp(ops_arr[0], "|") == 0) {
        int fd[2], fd2[2], pid1, pid2, pid3;
        int is_two_pipes = 0 ;
        if (pipe(fd) == -1) {
            printf("ERROR");
            exit(-1);
        }
        if(ops_num > 1){
            if(strcmp(ops_arr[1], "|") == 0){
                is_two_pipes = 1 ;
                if (pipe(fd2) == -1) {
                    printf("ERROR");
                    exit(-1);
                }
            }
        }
        pid1 = fork();
        if (pid1 == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execvp(args_lists[0][0], args_lists[0]);
            exit(0);
        }
        pid2 = fork();
        if (pid2 == 0) {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            if(is_two_pipes == 1){
                close(fd2[0]) ;
                dup2(fd2[1], STDOUT_FILENO) ;
                close(fd2[1]) ;
            }
            execvp(args_lists[1][0], args_lists[1]);
            exit(0);
        }
        if(is_two_pipes == 1){
            pid3 = fork() ;
            if(pid3 == 0){
                close(fd2[1]) ;
                dup2(fd2[0], STDIN_FILENO) ;
                close(fd2[0]) ;
                close(fd[0]) ;
                close(fd[1]) ;
                execvp(args_lists[2][0], args_lists[2]) ;
                exit(0) ;
            }
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        if(is_two_pipes == 1){
            close(fd2[0]) ;
            close(fd2[1]) ;
            waitpid(pid3, NULL, 0) ;
        }
    } else{
        execvp(args_lists[0][0], args_lists[0]) ;
    }
}



int main() {
    char command[1024];
    char *args_lists[4][10] ;
    char *ops_arr[3] ;
    int lists_num = 0, ops_num = 0 ;
    signal(SIGINT, ignore_sigint);
    while (1) {
        printf("command: ");
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0
        set_ops_arr(command, ops_arr, &ops_num) ;
        split_args(command, args_lists, &lists_num) ;
            /* Is command empty */
            if (lists_num == 0) {
                continue;
            } else if (lists_num == 1) {
                if (strcmp(args_lists[0][0], "exit") == 0) {
                    kill(0, SIGTERM);  // terminate all child processes
                }
                if (fork() == 0) {
                    signal(SIGINT, SIG_DFL); // set default behavior for SIGINT
                    execvp(args_lists[0][0], args_lists[0]);
                    fflush(stdout);
                    exit(0) ;
                }
                wait(NULL);
            } else {
                int temp_fd;
                dup2(STDOUT_FILENO, temp_fd);
                if (fork() == 0) {
                    signal(SIGINT, SIG_DFL); // set default behavior for SIGINT
                    handle_pipe(args_lists, ops_arr, ops_num);
                    exit(0) ;
                }
                wait(NULL);
                dup2(temp_fd, STDOUT_FILENO);
            }
    }
}
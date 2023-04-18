#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>


void handle_sigint(int sig) {
    printf("Received SIGINT, stopping tool...\nhello: ");
	fflush(stdout);
	
}
int main() {
	int fd[2];
	int i;
	int j = 0;
	int current_args_i = 0;
	int current_i = 0;
	char *argv[10];
	char command[1024];
	char *token;
	int index[3];
	char *my_arg[4][10];
	signal(SIGINT, handle_sigint);
	
	while (1) {
		
	    printf("hello: ");
	    fgets(command, 1024, stdin);
		printf("%d\n",command[0]);
	    command[strlen(command) - 1] = '\0'; // replace \n with \0
		
	    /* parse command line */
	    i = 0;
	    token = strtok (command," ");
		
	    while (token != NULL)
	    {
		printf("%s\n", token);
		
		argv[i] = token ;
		if( strcmp(token,">") == 0 || strcmp(token,">>") == 0 || strcmp(token,"|") == 0)
		{
			printf("%s\n",token);
			index[j] = i;
			j++;
			my_arg[current_args_i][current_i] = NULL ;
			current_args_i++ ;
			current_i = 0 ;
		}else{
			my_arg[current_args_i][current_i] = token;
			current_i ++ ;
		}
		token = strtok (NULL, " ");
		i++;
	    }
	    argv[i] = NULL;
		my_arg[current_args_i][current_i] = NULL;
	    /* Is command empty */ 
	    if (argv[0] == NULL)
		continue;

	    /* for commands not part of the shell command language */ 
	    if (fork() == 0) { 
			printf("%d\n",j);
			if(j==0) execvp(argv[0], argv);

			for (int i = 0; i < j; i++)
			{
				printf("shit000\n");
				if(strcmp(argv[index[i]], ">") == 0 ) 
				{
					printf("shit\n");
					FILE *file =  fopen(my_arg[i+1][0],"w");
					int fd = fileno(file);
					dup2(fd,STDOUT_FILENO);
					execvp(my_arg[i][0], my_arg[i]);
					fclose(file);
				}
				else if(strcmp(argv[index[i]], ">>") == 0 ) 
				{
					printf("shit3333\n");
					FILE *file =  fopen(my_arg[i+1][0],"a");
					int fd = fileno(file);
					dup2(fd,STDOUT_FILENO);
					execvp(my_arg[i][0], my_arg[i]);
					fclose(file);

				}
				else if(strcmp(argv[index[i]], "|") == 0 )
				{
					printf("pipeeeeeeeeeeeee\n");
					if(pipe(fd) == -1)
					{
						printf("ERROR");
						return -1;
					}
					if(fork() == 0)
						{
							close(fd[1]);
							dup2(fd[0],STDIN_FILENO);
							execvp(my_arg[i+1][0], my_arg[i+1]);	
						}
					else
					{
						close(fd[0]);
						dup2(fd[1],STDOUT_FILENO);
						execvp(my_arg[i][0], my_arg[i]);
						wait(NULL);
					} 

				}
			}
	    } 	
		wait(NULL);	
		i = j = current_args_i = current_i = 0;
		usleep(100000);
	}
}

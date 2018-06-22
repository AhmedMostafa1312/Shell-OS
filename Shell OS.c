#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#define BUFFER_SIZE           512
#define MAX_INPUT_LENGTH      512
int T;
unsigned char bgflag=0;  //initializing background flag status with 0

void parse(char *line, char **argv)  // Parses the line array into a vector of arguments
{
	while(*line != '\0')
	{
		while(*line == ' ' || *line == '\t' || *line == '&')  // Replace separators by terminators
		{	if((*line=='&' && *(line+1)=='\n')||(*line=='&' && *(line+1)==' ' && *(line+2)=='\n')||(*line=='&' && *(line+1)=='\t' && *(line+2)=='\n'))
				{
					bgflag=1;  //set the flag to run in background
					return;
				}
			else if(*line=='&')
				{
					break;	//break from if statement .. & is considered just an argument not a command
				}

			else if((*line==' ' && *(line+1)=='\n')||(*line=='\t' && *(line+1)=='\n'))
				{
					*line='\0';
                                        return;

				}

  			else
				{
					*line = '\0';
					line++;
				}

		}
		// Save the pointer to the argument
		*argv = line;
		argv++;
		while(*line != ' ' && *line != '\t' && *line != '\0')   // Skip over the rest of the characters of the argument
		{
			if((*line=='&' && *(line+1)=='\n')||(*line=='&' && *(line+1)==' ' && *(line+2)=='\n')||(*line=='&' && *(line+1)=='\t' && *(line+2)=='\n'))
				{
					bgflag=1;  //set the flag to run in background
					*line='\0';
				}
			if (*line =='\n')
				{
					*line = '\0';
				}
			line++;
		}
	}
	
	*argv = '\0';
} 

void execute(char **argv)  // Forks a child process to run execvp
{
	pid_t pid;
	pid = fork();
	if(pid < 0)  // Fork failed
		{
			printf("Error, no fork\n");
		}
	else if (pid == 0)  // In child process
		{
			execvp(argv[0], argv);
			T=execvp(argv[0], argv);
		        if (T<0 && argv[0]!='\0')
		        printf("command is not exist or cannot be executed\n");	
		}
	else  // In parent process
		{	if(bgflag==0)
				{
					wait(NULL); //in foreground
				}
			else
				{
					printf("This process is in background\n");  //in background
					bgflag=0; //set flag to 0 again for the next user commands
				}
		}

}
//---------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
/*
function name     : readcmd
description       : a function which prevents user of entering more than 512 characters in command line

inputs:
cmd               : char pointer : pointer which will carry the output of fgets function
maxInputsize      : integer which holds maximum length of characters to be entered
buffersize        : integer which holds maximum length that buffer can carry
*/
bool readcmd(char *cmd , int maxInputsize, int bufferSize)
{

	int counter = 0;
	fgets(cmd, bufferSize, stdin);
	while(cmd[counter] != '\0')
	{
		if(counter > maxInputsize)
			{
				printf("Input exceeds the max input size = %d \n", maxInputsize);
				return false;
			}
			counter ++;
	}
	return true;
}

int main()
{
	pid_t pid;
	char line[BUFFER_SIZE];
	char *argv[128];
	while(1)  // Main loop
	{
		printf("Shell>> ");
		if(readcmd(line, MAX_INPUT_LENGTH, BUFFER_SIZE) == true)
		{
			parse(line, argv);
                	
			//find(argv);

			if(strcmp(argv[0], "cd") == 0)   // An example of a shell built-in .. change directory
				{/*if (argv[1]=="--"){
                                 argv[1]="..";
                               }*/
                                if( chdir(argv[1])!=0){
                               printf("no such directory\n");}
                                 else{
					chdir(argv[1]);}
				}
			else if(strcmp(argv[0], "\0") == 0) // An example of empty command
				{
					continue;
				}
			else if(strcmp(argv[0], "exit") == 0) // An example of exit command
				{
					 kill(pid, SIGTERM);
                        		 sleep(2);
                        		 kill(pid, SIGKILL);
				}
			else
				execute(argv);
		}
	}

	return 0;
}

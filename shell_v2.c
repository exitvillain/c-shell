// This Shell Version 2 just adds piping functionality to our shell!  

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 32
#define MAX_ARGS 6

int main() {
    
    while (1) {
        
        //initialize two arrays of char stars 
        char *arguments[MAX_ARGS] =  {NULL};
        char *arguments2[MAX_ARGS] =  {NULL};
        
        //fgets will read into this buffer, so lets initialize it
        char buffer[BUFFER_SIZE] = {'\0'};

        //initialize array to pass into pipe
        //if no pipe exists, this array will do no harm.
        int fd[2] = {-1};
        
        //prompt the user 
        printf("$");  
    
        //get input from user. if we exit(1) on NULL, control D will work
        if (fgets(buffer,BUFFER_SIZE,stdin) == NULL) {
            exit(1);
        } 

        //tokenizer
        int index = 0;
        int inside = 0;  // 0 means outside the token
        int left = 0; // 0 means we are left of the pipe, if any
        char *saved_address = NULL;
        for(int i = 0; i < BUFFER_SIZE - 1; i++)
        {
            if (buffer[i] == '\n') {
                break;
            }
            else if (buffer[i] == ' ' || buffer[i] == '\0') {
                continue; 
            }
            else if (buffer[i] == '|') {
                left = 1;
                index = 0;
                continue;
            }
            else {
                if (inside == 0) {
                    inside = 1;
                    saved_address = &buffer[i];
                }
                
                if (buffer[i+1] == ' ' || buffer[i+1] == '\n' || buffer[i+1] == '\0') {
                    //replace with \0 so that the ends of the strings are marked
                    buffer[i+1] = '\0';

                    if(left == 0) {
                        arguments[index] = saved_address;
                        index = index + 1;
                        inside = 0;
                    }
                    else {
                        arguments2[index] = saved_address;
                        index = index + 1;
                        inside = 0;
                    }
                }    
            }
        }  
        if (arguments[0] == NULL) {
            continue;
        }
        if (left == 1 && arguments2[0] == NULL) {
            continue;
        }

        if (left == 1) {
            
            if (pipe(fd) < 0 ) {
                perror("pipe failed");
                continue;
            }
        }

        pid_t pid = fork();
        pid_t pid2 = -1000;

        if ((left == 1) && (pid > 0)) {
            pid2 = fork();    
        }

        if (pid < 0) {
            perror("fork failed");
            
        } else if (pid == 0 || pid2 == 0) { 
        // Child stuff
            if (left == 0) {
                
                if (execvp(arguments[0], arguments) < 0) {
                    perror("execvp failed");
                    exit(1); 
                }
            }
            else {
                 // if first child, we do this
                if (pid == 0) {
                    //close(fd[0]);
                    dup2(fd[1],1);
                    close(fd[0]);
                    close(fd[1]);
                    
                    if (execvp(arguments[0], arguments) < 0) {
                        perror("execvp failed");
                        exit(1); 
                    }
                }
                else {
                //if second child, we do this, we know pid wont be 0 
                //for second child, it will be pid of the parent. 
                    dup2(fd[0],0);
                    close(fd[1]);
                    close(fd[0]);
                    if (execvp(arguments2[0], arguments2) < 0) {
                        perror("execvp failed");
                        exit(1); 
                    }
                }
            }
        } else {
            // Parent Stuff

            // the parent closes both ends of the pipe
            if (left == 1) {
                close(fd[0]);
                close(fd[1]);
            }
            while (wait(NULL) > 0); // Reaps all children

        }
       
    }
}

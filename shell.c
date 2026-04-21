#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 32
#define MAX_ARGS 6


int main() {
    
    while (1) {
        
        //initialize array of char stars  
        char *arguments[MAX_ARGS] =  {NULL};
        
        //Initialize input buffer with null characters. This is a good way to clear out the buffer before each shell prompt. 
        char buffer[BUFFER_SIZE] = {'\0'};
        
        //prompt the user 
        printf("$");  
    
        //get input from user. if we exit(1) on NULL, control D will work
        if (fgets(buffer,BUFFER_SIZE,stdin) == NULL) {
            exit(1);
        } 

        //tokenizer
        int index = 0;
        int inside = 0;  // 0 means outside the token
        char *saved_address = NULL;
        for(int i = 0; i < BUFFER_SIZE - 1; i++)
        {
            if (buffer[i] == '\n') {
                break;
            }
            else if (buffer[i] == ' ' || buffer[i] == '\0') {
                continue; 
            }
            else {
                if (inside == 0) {
                    inside = 1;
                    saved_address = &buffer[i];
                }
                if (buffer[i+1] == ' ' || buffer[i+1] == '\n' || buffer[i+1] == '\0') {
                    buffer[i+1] = '\0';
                    arguments[index] = saved_address;
                    index = index + 1;
                    inside = 0;
                }    
            }
        }  
        
        if (arguments[0] == NULL) {
            continue;
        }
        pid_t pid = fork();
          
        if (pid < 0) {
            perror("fork failed");
            
        } else if (pid == 0) { 
        // Child stuff
            if (execvp(arguments[0], arguments) < 0) {
                perror("execvp failed");
                exit(1); 
            }
        } else {
            // Parent Stuff
            wait(NULL);
        }

    }
}

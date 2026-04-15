#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    
    while (1) {
         
        //initialize array of char stars and a regular array 
        char *arguments[6] =  {NULL};
        
        //Initialize input buffer, 
        char buffer[32];
        
        //prompt the user 
        printf("$");  
    
        //get input from user, lets put it in a buffer
        fgets(buffer,32,stdin);

        //tokenizer
        int index = 0;
        int inside = 0;  // 0 means outside the token
        char *saved_address = NULL;
        for(int i = 0; i < 31; i++)
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

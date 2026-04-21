// this version adds output and input redirection usng > and <  

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

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
      
        int index = 0;   //this will serve as the index value to both arguments arrays
        int inside = 0;  // 0 is outside the token, 1 is inside
        int left = 0; // 0 is left of the | or >  < if any, 1 is right of 
        int redirect = 0; // 0 means not a redirection scenario, 1 is > , 2 is <
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
            else if (buffer[i] == '>') {
                left = 1; 
                index = 0;
                redirect = 1;
                continue;
            }
            else if (buffer[i] == '<') {
                left = 1;
                index = 0;
                redirect = 2;
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
// End of Tokenizer.Now that we got user input, We are locked and loaded, ready to execute commands
      
        if (arguments[0] == NULL) {
            continue;
        }
        if (left == 1 && arguments2[0] == NULL) {
            continue;
        }

        if (left == 1 && redirect == 0) {
            
            if (pipe(fd) < 0 ) {
                perror("pipe failed");
                continue;
            }
        }

        pid_t pid = fork();
        pid_t pid2 = -1000;

        if ((left == 1) && (pid > 0) && (redirect == 0)) {
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

                    if (redirect == 1) {
                        int fd2 = open(arguments2[0],O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd2 == -1) {
                            perror("Error opening output file");
                            exit(1);
                        }
                        dup2(fd2,1);
                        close(fd2);
                    }
                    else if (redirect == 2) {
                        int fd2 = open(arguments2[0],O_RDONLY);
                        if (fd2 == -1) {
                            perror("Error opening input file");
                            exit(1);
                        }
                        dup2(fd2,0);
                        close(fd2);
                    }
                    else {
                    
                        dup2(fd[1],1);
                        close(fd[0]);
                        close(fd[1]);
                    }
                    
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
            if (left == 1 && redirect == 0) {
                close(fd[0]);
                close(fd[1]);
            }
            while (wait(NULL) > 0); // Reaps all children

        }
       
    }
}

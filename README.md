## Summary

This project is a minimal Unix-like shell written in C that demonstrates how command execution works at a low level.

It takes raw user input, tokenizes it into arguments, and executes commands using fork and execvp.

The goal of this project is to understand the exact mechanics behind how shells, reverse shells, and command execution work under the hood — without relying on external libraries or abstractions.

## Features Section
⚙️ Features
Interactive command prompt ($)
Custom tokenizer (no strtok or libraries)
In-place string parsing using '\0' termination
Argument vector construction (argv-style)
Process creation using fork()
Command execution using execvp()
Parent process synchronization using wait()
Basic error handling using perror()

🔐 Why This Matters (Security & Systems)

Understanding how input becomes commands and how commands become processes is critical for:

Command injection vulnerabilities
Remote code execution (RCE)
Reverse shells and web shells
Post-exploitation workflows

This project strips away all abstraction and shows exactly what happens when a system executes a command.

## Example usage
▶️ Example
$ ls -la
$ pwd
$ whoami

🛠️ Build & Run
gcc shell.c -o shell
./shell

⚠️ Limitations
Fixed input buffer size (32 characters)
Limited number of arguments
No support for pipes (|)
No redirection (>, <)
No command history or autocomplete

🚀 Future Work
Add pipe support (|)
Add input/output redirection
Windows equivalent using process APIs (C#)

🔍 How It Works
Read user input using fgets
Tokenize input by replacing delimiters with '\0'
Store pointers to tokens in an argument array
Call fork() to create a child process
Execute command in child using execvp()
Parent waits for child using wait()

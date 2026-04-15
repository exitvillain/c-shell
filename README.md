## Summary

This project is a minimal Unix-like shell written in C that demonstrates how command execution works at a low level.

It takes raw user input, tokenizes it into arguments, and executes commands using `fork` and `execvp`.

The goal of this project is to understand the exact mechanics behind how shells, reverse shells, and command execution work under the hood — without relying on external libraries or abstractions.

---

## Features

- Interactive command prompt (`$`)
- Custom tokenizer (no `strtok` or libraries)
- In-place string parsing using `'\0'` termination
- Argument vector construction (`argv`-style)
- Process creation using `fork()`
- Command execution using `execvp()`
- Parent process synchronization using `wait()`
- Basic error handling using `perror()`

---

## Why This Matters (Security & Systems)

Understanding how input becomes commands and how commands become processes is critical for:

- Command injection vulnerabilities
- Remote code execution (RCE)
- Reverse shells and web shells
- Post-exploitation workflows

This project strips away abstraction and shows exactly what happens when a system executes a command.

---

## Example

$ ls -la
$ ls -l /tmp
$ cat shell.c
$ pwd
$ whoami

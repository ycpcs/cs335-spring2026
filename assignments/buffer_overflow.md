---
layout: default
course_number: CS335
title: Lab - Buffer Overflow
---

# Lab: Buffer Overflow Attack

## Learning Objectives
By the end of this lab, you will be able to:

- Explain how stack-based buffer overflows occur
- Analyze stack memory layout on 32-bit Linux systems
- Exploit a buffer overflow to redirect program control flow
- Construct shellcode-based exploits against Set-UID programs
- Understand and evaluate modern defense mechanisms such as stack canaries and ASLR

## Ethical Use Notice
This lab is intended **only for educational use in a controlled environment**. The techniques demonstrated here must not be applied to systems you do not own or have explicit permission to test.

## Background
A buffer overflow occurs when a program writes more data to a memory buffer than it was allocated to hold. On systems that do not enforce proper memory safety, this can allow attackers to overwrite adjacent memory regions, including control-flow data such as return addresses. Historically, buffer overflows have been a major source of real-world vulnerabilities.

This lab explores buffer overflow attacks in a **32-bit Linux environment**, where stack-based exploitation is easier to study due to predictable calling conventions and limited address-space randomization.

- <a href="..\schedule\papers\buffer-overflow.pdf" target="_blank">Buffer Overflow Attack</a>
- <a href=".\buffer\stack_smashing.pdf" target="_blank">Smashing The Stack For Fun And Profit, Aleph One</a>

## Part 1: Analyzing a Hidden Backdoor in Legacy Code
------------------------------------------------------------
On March 10, 2016, Halcyon Dynamics detected unauthorized outbound network activity originating from a staging server. Investigation revealed that an internal utility contained a hidden function capable of initiating a reverse shell connection. This function, left behind by a former employee, became reachable due to a buffer overflow vulnerability in the same program. Although no customer data was accessed, the presence of a backdoor combined with a memory‑safety flaw created a significant security risk. The incident was contained quickly, and remediation efforts are ongoing.

### Technical Summary 
The compromised component was an internal command‑line tool and two critical issues were identified: 
#### 1. Buffer Overflow Vulnerability 
- User input was copied into a fixed-size stack buffer using `strcpy()`
- The buffer was only **5 bytes long**
- No bounds checking was performed
- Overlong input overwrote adjacent stack memory, including the saved return address
 
##### 2. Hidden Reverse Shell Backdoor 
- A function named `call_home()` was present in the binary
- The function was never called during normal execution
- It contained logic to initiate a reverse shell connection
- By exploiting the buffer overflow, execution could be redirected to this function

#### Your Task
You are provided with the source file `bof_rs.c`, which contains the unused function `call_home()`.

Your objective is to:
- Analyze the program’s stack layout
- Determine how a buffer overflow can redirect execution to `call_home()`
- Explain the conditions required for this redirection to succeed

**Important constraints:**
- You may not modify `bof_rs.c`
- You must explain *how* and *why* the exploit works

### Environment Configuration
To simplify analysis, disable the following protections:

Disable stack protection and allow executable stack:
```bash
-fno-stack-protector -z execstack
```
To turn off Address Space Layout Randomization (ASLR): ```$ sudo sysctl -w kernel.randomize_va_space=0```
  - Check the current randomization status: ```$ sudo sysctl kernel.randomize_va_space```
Compile the program in 32-bit mode using the ```-m32`` flag

### Report Requirements (Part 1)
Include the following in your report:

- The exact command used to run the program
- Terminal output showing the execution of `call_home()`
- Screenshots of any debugging sessions used to determine exploit parameters
- A clear explanation of how the buffer overflow alters control flow


### Part 2: Buffer Overflow Attack Lab on Set-UID programs
------------------------------------------------------------
In this part, you will exploit a buffer overflow in a **Set-UID root program** to obtain a root shell.


### Environment Setup
Modern operating systems have implemented several security mechanisms to make the buffer-overflow attack difficult. To simplify our attacks, we need to disable them first. Later on, we will enable them and see whether our attack can still be successful or not.

#### Disable ASLR
Ubuntu and several other Linux-based systems uses address space randomization to randomize the starting address of heap and stack. This makes guessing the exact addresses difficult; guessing addresses is one of the critical steps of buffer-overflow attacks. This feature can be disabled using the following command:
```bash
sudo sysctl -w kernel.randomize_va_space=0
```

#### Configure `/bin/sh`
Modern Linux systems link `/bin/sh` to `dash`, which drops privileges when executed from a Set-UID process. To avoid this behavior, relink `/bin/sh` to `zsh`:

```bash
sudo ln -sf /bin/zsh /bin/sh
```

#### Task 1 — Getting Familiar with Shellcode
Shellcode is a small piece of machine code used as the payload in code-injection attacks. In this lab, shellcode launches a shell that inherits the privileges of the vulnerable program. You are provided with assembly shellcode that executes `/bin/sh`.

##### C Version of Shellcode
The file `call_shellcode.c` copies shellcode onto the stack and executes it.
```c
#include <stdio.h>

int main() {
    char *name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve(name[0], name, NULL);
}
```
Unfortunately, we cannot just compile this code and use the binary code as our shellcode. The best way to write a shellcode is to use assembly code. In
this lab, the shellcode is provided. 

##### Running the Provided Shellcode Program
The file *call_shellcode.c* copies shellcode onto the stack and executes it.

Compile using the `Makefile`:

- a32.out → 32‑bit version
- a64.out → 64‑bit version

Run both and *observe* the behavior.

#### Task 2 — Understanding the Vulnerable Program

The vulnerable program is *stack.c*. It reads 517 bytes from *badfile* and copies them into a much smaller buffer using `strcpy()`. Your job is to exploit this vulnerability and gain the root privilege.

##### Vulnerable Code
```c
#ifndef BUF_SIZE
#define BUF_SIZE 100
#endif

int bof(char *str) {
    char buffer[BUF_SIZE];
    strcpy(buffer, str);   // Vulnerability
    return 1;
}

int main(int argc, char **argv) {
    char str[517];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 517, badfile);
    bof(str);

    printf("Returned Properly\n");
    return 1;
}
```

##### Why It’s Vulnerable

- ```strcpy()``` does not check bounds
- Input is larger than the `buffer`
- Overflow allows overwriting the return address
- Program runs with root privileges (Set-UID)

##### Compilation Requirements

The `Makefile` compiles the program with the following properties:
- 32-bit executable
- Executable stack
- Stack protection disabled
- Set-UID root permissions

You only need to run:
```bash
make
```

**Important** In the `Makefile`, the `BUF SIZE` value is set by four variables L1, ..., L4.
**Set the following:**
- L1: 240
- L2: 160

#### Task 3 — Launching Attack on 32‑bit Program (Known Buffer Size - L1)

##### Investigating Stack Layout with GDB
Use `gdb` to determine the distance between:
- The start of `buffer`
- The saved return address

Suggested commands:
```bash
touch badfile
gdb stack-L1-dbg
b bof
run
p $ebp
p &buffer
```

**Note**: GDB changes the stack layout, so addresses differ from real execution. This is because GDB has pushed some environment data into the
stack before running the debugged program. When the program runs directly without using GDB, the stack does not have those data, so the actual frame pointer value will be larger. You should keep this in mind when constructing your payload.

##### Crafting the Exploit

To exploit the buffer-overflow vulnerability in the target program, we need to prepare a payload, and save it inside *badfile*. We will use a Python program to do that. A skeleton program called ```exploit.py```, which is included in the lab setup file. The code is incomplete, and you need to replace some of the essential values in the code.

```python
shellcode = (
    ""  # Insert shellcode here
).encode('latin-1')

content = bytearray(0x90 for i in range(517))

start = 0
content[start:start+len(shellcode)] = shellcode

ret = 0x00
offset = 0
L = 4

content[offset:offset+L] = (ret).to_bytes(L, byteorder='little')

with open('badfile', 'wb') as f:
    f.write(content)
```

You must determine:

- Shellcode placement
- Correct return address
- Offset to saved return address

A successful exploit yields a root shell. In your report, in addition to providing screenshots to demonstrate your investigation and attack, you also need to explain how the values used in your ```exploit.py``` are decided.

#### Task 4 - Launching Attack Without Knowing Buffer Size (L2)

In this level, you cannot rely on knowing *BUF_SIZE*.

Constraints:
------------

- You may use GDB, but not to determine buffer size
- You must not read the Makefile
- Assume buffer size is between 100 and 200 bytes
- Exploit must work for any size in this range

Goal:
-----

Create a payload that:

- Places shellcode reliably in memory
- Overwrites the return address without knowing buffer length
- Avoids brute force

Hints:
------

- Stack frame alignment is predictable
- Place shellcode near the end of the input buffer
- Overwrite a wide range of possible return addresses
- Use a NOP sled to increase reliability

#### Task 5 - Defeating Address Space Layout Randomization (ASLR)
------------------------------------------------------------

On 32-bit Linux machines, stacks only have 19 bits of entropy, which means the stack base address can 2^19 = 524, 288 possibilities. This number is not that high and can be exhausted easily with the brute-force approach. In this task, we use such an approach to defeat the address randomization countermeasure on our 32-bit VM. First, we turn on the Ubuntu’s address randomization using the following command. 
```bash
$ sudo /sbin/sysctl -w kernel.randomize_va_space=2
```
Then we run the same attack against ```stack-L1```. Please describe and explain your observation.

We then use the brute-force approach to attack the vulnerable program repeatedly, hoping that the address we put in the badfile can eventually be correct. We will only try this on ```stack-L1```, which is a 32-bit program. You can use the following shell script to run the vulnerable program in an infinite loop. If your attack succeeds, the script will stop; otherwise, it will keep running. Please be patient, as this may take a few minutes, but if you are very unlucky, it may take longer. Please describe your observation.

```bash
#!/bin/bash
SECONDS=0
value=0
while true; do
value=$(( $value + 1 ))
duration=$SECONDS
min=$(($duration / 60))
sec=$(($duration % 60))
echo "$min minutes and $sec seconds elapsed."
echo "The program has been running $value times so far."
./st
```
------------------------------------------------------------

### Grading

- Post your report in <a href="https://cs.ycp.edu/marmoset" target="_blank">Marmoset</a> by the scheduled due date in the syllabus. Your grade for this lab will be composed of:
- You need to submit a detailed lab report, with **screenshots**, to describe what you have done and what you have observed. You also need to provide explanation to the observations that are interesting or surprising. Please also list the important code snippets followed by explanation. Simply attaching code without any explanation will not receive credits.
  - 30% - Design:  The overall quality and structure of your exploit.
  - 30% - Observations: Insightfulness and depth in your understanding of the task
  - 40% - Explanation: Clarity and completeness of your report and analysis.
  - *Extra Credit*: Additional investigation beyond the requirements.

<!--
1	Familiar with Shell 5
2	Vulnerable program 5
3	Level 1 on 32 bit	30
4	Level 2 on 32 bit 30
7	dash countermeasures 15
8	ASLR 7.5
9	Other Protections	7.5
->
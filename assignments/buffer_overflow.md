---
layout: default
course_number: CS335
title: Lab - Buffer Overflow
---

# Lab - Buffer Overflow Attack Lab

## Lab Overview 
In this lab, you will explore the concept of a ```buffer overflow attack```, which is a common security vulnerability in software. By exploiting a buffer overflow, an attacker can manipulate the program's execution flow, potentially executing arbitrary code.
  - <a href="..\schedule\papers\buffer-overflow.pdf" target="_blank">Buffer Overflow Attack</a>
  - <a href=".\buffer\stack_smashing.pdf" target="_blank">Smashing The Stack For Fun And Profit, Aleph One</a>

### Part 1: Analyzing a Hidden Backdoor in Legacy Code
On March 10, 2016, Halcyon Dynamics detected unauthorized outbound network activity originating from a staging server. Investigation revealed that an internal utility contained a hidden function capable of initiating a reverse shell connection. This function, left behind by a former employee, became reachable due to a buffer overflow vulnerability in the same program. Although no customer data was accessed, the presence of a backdoor combined with a memory‑safety flaw created a significant security risk. The incident was contained quickly, and remediation efforts are ongoing.

#### Technical Summary 
The compromised component was an internal command‑line tool and two critical issues were identified: 
##### 1. Buffer Overflow Vulnerability 
The program used an unsafe string copy operation (`strcpy`) to move user‑supplied input into a fixed‑size buffer of only five bytes. Because no bounds checking was performed, inputs longer than the buffer allowed overwrote adjacent stack memory, including control‑flow data such as the saved return address. 
##### 2. Hidden Reverse Shell Backdoor 
A dormant function named `call_home()` remained compiled into the binary. Although not invoked during normal execution, the function contained logic to initiate a reverse shell connection to a remote host. The buffer overflow made it possible for an attacker to redirect execution into this function. Together, these issues allowed an external actor to trigger the backdoor by exploiting the overflow, resulting in unauthorized outbound connection attempts from the staging environment.

#### Tasks 
In this lab, you will analyze the provided [bof_rs.c](buffer\bof_rs.c) program, which contains an unused function named ```call_home()```. Your objective is to understand how a buffer overflow can alter program control flow and make dormant code reachable. By examining the program’s memory layout and behavior, you will determine how an attacker could redirect execution into ```call_home()``` and explain the conditions required for this to occur.
- To avoid Segmentation Faults caused by stack protection, disable the stack guard:
  - ```-fno-stack-protector -z execstack```
- To turn off Address Space Layout Randomization (ASLR): ```$ sudo sysctl -w kernel.randomize_va_space=0```
  - Check the current randomization status: ```$ sudo sysctl kernel.randomize_va_space```
- Do not forget to compile the program in 32‑bit mode using the ```-m32`` flag
- 
- **Important**: No modifications are allowed to the ```bof_rs.c``` program itself.
  - You will need to capture the following in your report:
    - The exact command you used to run the program.
    - Terminal output showing the function's execution.
    - Screenshots of any debugging sessions you used to determine the correct command format.
  
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
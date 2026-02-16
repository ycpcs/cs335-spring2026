---
layout: default
course_number: CS335
title: Assignment 2
---

# Assignment – Ethernet, IP & ICMP and UDP
This assignment explores the fundamental protocols of computer networking, including Ethernet, ARP, IP, ICMP, and UDP, through hands-on experimentation in a Docker-based environment. The assignment also examines common network attacks—such as ARP poisoning, IP fragmentation attacks, and UDP amplification—to understand protocol vulnerabilities and security implications.

--- 
## Lab Environment and hints 
You may use any of the `Labsetup.zip` files or `docker-compose.yml` configurations used in class. You are expected to demonstrate your work using Docker containers and network tools (e.g., netcat, Python scripts, Wireshark).

---
## Documentation & Screenshot Requirements (IMPORTANT)

Your lab report must include screenshots where necessary to demonstrate your work. Failure to include proper screenshots will result in grade deductions.

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

#### `docker exec -it <container id> /bin/bash`
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
## Part I - Ethernet 

### Problem 1
Assume machines `A` and `B` are on the same network `10.3.2.0/24`.
Machine `A` sends spoofed IP packets.
- When `A` spoofs packets with destination `1.2.3.4`, `B` can observe them.
- When `A` spoofs packets with destination `10.3.2.30`, `B` cannot observe them.

There is nothing wrong with the spoofing or sniffing program. Explain why this behavior occurs. Why are packets to `10.3.2.30` never actually sent out?

### Problem 2
A computer with IP `10.8.8.5/24` tries to ping `10.8.8.8`. An ARP request is sent first. What are the values of:
- Destination MAC address (Ethernet header)
- Source MAC address (Ethernet header)

Explain your reasoning.

### Problem 3
A news report claims that outsiders sent spoofed ARP requests/responses from remote machines over the Internet to launch ARP cache poisoning attacks against York College’s internal network. Is this technically possible? Explain clearly.

### Problem 4
A computer with IP `10.8.8.5/24` tries to ping `10.8.8.100`, which does not exist on the LAN. Another machine on the same LAN is sniffing traffic. Will it be able to see the ICMP echo request? Why or why not?

### Problem 5
Write a Python code snippet (Scapy recommended) that sends a gratuitous ARP reply to poison the ARP cache of machine `10.8.8.5`. Your attack machine is on the same LAN. Include:
- The Python code
- A screenshot proving the ARP table was poisoned

### Problem 6
In the MITM attack demonstrated in class, attacker `M` uses ARP poisoning to redirect `A <-> B` traffic.
- If IP forwarding on `M` is off, what happens to the packets?
- If IP forwarding on `M` is on, what happens?
- If the attacker wants to modify packets, should IP forwarding be on or off? Explain.

### Problem 7
A host on `10.8.8.0/24` has default gateway `10.8.8.1`. It tries to ping `93.184.216.34`. Before sending the ICMP packet, it sends an ARP request. What is the target IP address in the ARP request? Explain why.

---
## Part II — IP & ICMP 

### Problem 1
Why is IP fragmentation necessary?

### Problem 2
Without receiving the last fragment, can a receiver determine the total size of the original IP datagram? Explain your answer.

### Problem 3
The IP Total Length field is `16` bits. Is it possible to spoof an IP packet larger than `65,535` bytes? If not, explain why.
If yes (in some indirect way), explain the mechanism.

### Problem 4
Please create two packets to emulate the Teardrop attack. Provide:
- Python code
- A brief explanation of how the overlap occurs

### Problem 5
Please create two packets to emulate the Ping-of-Death attack. Provide:
- Python code
- A brief explanation of how the overlap occurs

### Problem 6
What is reverse path filtering? What is the purpose of such a mechanism?

### Problem 7
Under what conditions does a router send an ICMP Redirect message?

### Problem 8 
Can an ICMP redirect attack redirect traffic to a machine outside the LAN? Explain why or why not.

### Problem 9 
The subnet is `10.0.2.0/24`. The directed broadcast address is `10.0.2.255`. Explain how directed broadcast can be abused to perform a denial-of-service attack. Name one historical attack associated with this technique.

---

## Part 3 - UDP 

### Problem 1
Why are ports `0–1023` restricted to root (privileged) users in Linux?

### Problem 2
In the following code:

```python
udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp.bind(("0.0.0.0", 9090))

```

What does `0.0.0.0` mean?

### Problem 3
What are the similarities between the Fraggle attack and the Smurf attack?

### Problem 4
An open-source program:
- Receives a UDP message
- Immediately sends a reply to the sender

Alice runs it on:
- `10.8.0.8:8000`

Bob runs it on:
- `192.168.0.7:7000`

1. Write a simple Python program to trigger a UDP ping-pong loop between them.
2. Explain how the program could be modified to prevent this vulnerability.

### Problem 5
UDP services are commonly used in amplification attacks. Why is TCP generally not suitable for amplification attacks?

---
## Grading Submission
Submit your lab report, including **screenshots**, via [Marmoset](https://cs.ycp.edu/marmoset) by the scheduled due date listed in the syllabus.
Your submission must include:
- Written answers
- Python code
- Command-line screenshots
- Wireshark screenshots
- Clear explanations of what each screenshot demonstrates

Reports without sufficient proof of experimentation will receive reduced credit.
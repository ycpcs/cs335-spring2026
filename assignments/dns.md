---
layout: default
course_number: CS335
title: Lab - DNS Attacks
---

# Lab: DNS Attacks

## Learning Objectives
DNS functions as the Internet’s phonebook, mapping human‑readable hostnames to numerical IP addresses (and sometimes performing reverse lookups). This translation happens automatically through the DNS resolution process. DNS attacks interfere with this process by manipulating the information a resolver receives, with the goal of redirecting users to unintended—and often malicious—destinations. The purpose of this lab is to help students understand how these attacks work in practice. Students will configure their own DNS server and then carry out a series of controlled DNS‑based attacks within an isolated lab environment.

This lab focuses on local DNS attacks and includes the following topics:
- How DNS operates and how name resolution is performed
- Setting up and configuring a DNS server
- DNS cache‑poisoning techniques
- Crafting and spoofing DNS responses
- Capturing and manipulating packets
- Using Scapy for packet construction and injection

## Ethical Use Notice
This lab is intended **only for educational use in a controlled environment**. The techniques demonstrated here must not be applied to systems you do not own or have explicit permission to test.

## Lab Environment 
Setup files: [Labsetup.zip](dns\Labsetup.zip)

The main target of DNS cache‑poisoning experiments is the local DNS resolver, but attacking any real production server would be illegal and unethical. To conduct these experiments safely, you must set up your own controlled environment. This lab requires four separate hosts, each playing a distinct role in the attack scenario:

### Roles in the Lab Environment
- User (10.9.0.5) — generates DNS queries that will be intercepted or spoofed.
- Local DNS server (10.9.0.53)  — the resolver whose cache you will attempt to poison.
- Attacker machine #1  (10.9.0.1) — sends spoofed DNS responses.
- Attacker machine #2  (10.9.0.153) — attacker's nameserver.

The user container at `10.9.0.5` is already configured to use `10.9.0.53` as its local DNS resolver. This is done by modifying the machine’s resolver configuration file, `/etc/resolv.conf`, so that `10.9.0.53` appears as the first nameserver entry. Because the resolver consults entries in order, this ensures that `10.9.0.53` is treated as the primary DNS server for all name‑resolution requests originating from the user machine.

Attacker’s nameserver hosts two separate DNS zones: the legitimate `attacker32.com` zone and a forged `example.com` zone. Both zones must be defined on the attacker’s DNS server so it can respond authoritatively for each domain during the experiment. These zone configurations are added to the server’s main BIND configuration file, located at `/etc/bind/named.conf`.

This separation mirrors real‑world DNS infrastructure while ensuring that all experimentation occurs within an isolated, fully authorized environment. It also allows students to observe how different components interact during a cache‑poisoning attempt and to analyze the behavior of the resolver under controlled conditions.

## Task 1 – Directly Spoofing DNS Responses to the User
### Objective
The goal of this task is to perform a DNS response spoofing attack, where the attacker sends a fake DNS response to the user before the legitimate DNS server replies.

### Background
When a user attempts to access a website (a host name, such as `www.example.com`) , the system sends a DNS query to the configured DNS server. The server then replies with the corresponding IP address.

If an attacker is located on the same local network, they can sniff the DNS request packets sent by the user. Once the request is captured, the attacker can immediately generate a fake DNS response packet and send it back to the user.

If the spoofed response arrives before the legitimate response and matches the query’s transaction ID and parameters, the user machine will accept the spoofed response and ignore subsequent responses. This attack works because the DNS protocol does not inherently authenticate responses.

### Attack Procedure
Monitor DNS queries on the network using a packet sniffing tool.
Detect DNS requests sent by the user machine.
Craft a spoofed DNS response packet that:
- Appears to originate from the DNS server
- Contains the same transaction ID as the request
- Provides a malicious IP address for the queried domain
- Send the spoofed packet to the user before the legitimate response arrives

### Implementation
Students will use the Scapy tool to create and send forged DNS packets. A code skeleton is provided: 

```bash 
#!/usr/bin/env python3
from scapy.all import *
import sys

NS_NAME = "example.com"

def spoof_dns(pkt):
    if (DNS in pkt and NS_NAME in pkt[DNS].qd.qname.decode("utf-8")):
        print(pkt.sprintf("{DNS: %IP.src% --> %IP.dst%: %DNS.id%}"))
        ip = IP(...)          # Create an IP object
        udp = UDP(...)        # Create a UDP object
        Anssec = DNSRR(...)   # Create an answer record
        dns = DNS(...)        # Create a DNS object
        spoofpkt = ip/udp/dns
        send(spoofpkt)

myFilter = "..."  # Set the filter
pkt = sniff(iface="br-43d947d991eb", filter=myFilter, prn=spoof_dns)
``` 

Replace the `iface` argument with the actual network interface associated with the `10.9.0.0/24` network (e.g., `eth0`, `eth1`, etc).

#### Verifying the attack
While the attack program is running on the attacker machine, DNS queries should be generated from the user (victim) machine. This can be done using the `dig` command.

Running this command causes the user machine to send a DNS query to the local DNS server. If the DNS server does not already have the requested record in its cache, it will forward the query to the authoritative name server for the domain (e.g., `example.com`).

During this process, the attacker program attempts to inject a spoofed DNS response. If the attack is successful, the reply received by the user machine will contain the spoofed information generated by the attacker, such as a malicious IP address.

Students should compare the DNS query results before running the attack and after the attack is executed in order to verify whether the spoofing attack was successful.

#### Clearing the DNS Cache

Before launching the attack, it is important to ensure that the cache on the local DNS server is cleared.

If the DNS server already has the requested record stored in its cache, it will immediately return the cached result to the user. In this situation, the legitimate response from the DNS server will reach the user much faster than any spoofed response generated by the attacker, causing the attack to fail.

To avoid this issue, flush the DNS cache on the local DNS server before starting the experiment. This ensures that the DNS server will forward the query externally, giving the attack program an opportunity to inject a spoofed response.

```bash 
# rndc flush // Flush the DNS cache
```

## Task 2 – DNS Cache Poisoning Attack

### Objective
The goal of this task is to poison the cache of the local DNS server, allowing attackers to redirect multiple users to a malicious destination.

### Background
The previous attack targets only the user's machine. To maintain the effect, the attacker must send a spoofed DNS response every time the user queries `www.example.com`. This approach is inefficient and unreliable. A far more effective strategy is to target the local DNS server instead of the individual user.

When a DNS server receives a query, it first checks its cache. If the answer is already cached, the server immediately returns that information to the user. If the answer is not cached, the server queries external DNS servers to obtain it. Once the response is received, the DNS server stores it in its cache so that future queries can be answered locally without contacting external servers.

Because of this behavior, if an attacker can successfully spoof the response that the DNS server expects from external authoritative servers, the DNS server will store the forged information in its cache. From that point on, any user querying the same hostname will receive the attacker’s spoofed response directly from the cache. This means the attacker only needs to succeed once, and the malicious result will persist until the cached entry expires.

This technique is known as *DNS cache poisoning*.

### Attack Strategy & Verification
Please modify the program used in the previous task for this attack. Before attacking, make sure that the DNS server’s cache is empty.

Check whether the DNS cache has been poisoned by running:
- `dig www.example.com`
- If the attack succeeds, the DNS server will return the malicious IP address.
- You can check whether the local DNS server has been successfully poisoned by examining its cache. The following
commands first dump the cache into a file, and then display the content of the cache file.

```bash
# rndc dumpdb -cache
# cat /var/cache/bind/dump.db
```

## Task 3 – DNS Cache Poisoning: Targeting the Authority (NS) Section

### Objective
In this task, students will attempt to poison the authority section of DNS responses.

### Background
A DNS response may contain multiple sections:
- Answer section – contains the requested IP address.
- Authority section – specifies authoritative name servers for the domain.
- Additional section – may include IP addresses for those name servers.

Instead of modifying the answer section directly, attackers can manipulate the authority section to specify malicious name servers controlled by the attacker.

Once the DNS server trusts these malicious name servers, future queries for that domain will be directed to the attacker’s server.

### Attack Strategy & Verification 
Please modify your DNS spoofing program so that the forged DNS response includes a spoofed NS record. Once this malicious NS record is cached, all future queries for any hostname under `example.com` — including `www.example.com`, `mail.example.com`, or anything else—will be directed to `ns.attacker32.com`. Because this nameserver is controlled by the attacker, it can provide forged responses for any query. In our setup, the attacker‑controlled nameserver is located at IP address `10.9.0.153`.

```bash 
;; AUTHORITY SECTION:
example.com. 259200 IN NS ns.attacker32.com.
```

Before running your attack, make sure to clear the DNS cache on the local DNS server. This ensures that no existing entries interfere with your results.

After launching the attack, use the `dig` command on the user machine to query any hostname within the example.com domain. If your attack succeeds, the response should contain the fake IP address associated with `ns.attacker32.com`, indicating that the resolver has accepted your spoofed NS record.

Finally, inspect the DNS cache on the local DNS server to verify whether the spoofed NS record has been stored. This confirms that the resolver has cached the malicious delegation information, not just the forged answer.

## Task 4 – Targeting Another Domain

### Objective

This task demonstrates that attackers may poison DNS entries for domains that were not directly queried.

### Background

A DNS response may contain information about other domains in the authority or additional sections. Some DNS servers may cache these records even if they were not explicitly requested.

Attackers can exploit this behavior by including malicious entries for unrelated domains in the response.

### Attack Strategy & Verification 

In the previous attack, you successfully poisoned the cache of the local DNS server so that `ns.attacker32.com` became the nameserver for the `example.com` domain. Building on that result, we now want to explore whether the same spoofed response can be used to influence the DNS server’s behavior for other domains as well.

In this task, you will modify your forged DNS response—still triggered by a query for `www.example.com` - to include an additional NS record in the Authority section. Specifically, you will attempt to make the DNS server believe that `ns.attacker32.com` is also the nameserver for `google.com`:

```bash 
;; AUTHORITY SECTION:
example.com.   259200   IN   NS   ns.attacker32.com.
google.com.    259200   IN   NS   ns.attacker32.com.
```

Update your attack program so that it includes both of these NS records in the Authority section, and then launch the attack against your local DNS server.

After the attack completes, inspect the DNS cache on the local DNS server and determine which of the NS records, if any, were actually cached. Provide an explanation for your observations. Remember that the only query you are spoofing is for a hostname under `example.com` — you are not attacking a query for `google.com` itself.

## Task 5 – Targeting the Additional Section

### Objective

The goal of this task is to exploit the additional section of DNS responses to inject malicious information into the DNS cache.

### Background

The additional section of a DNS response may include extra records such as the IP addresses of name servers. If a DNS server caches this information, attackers can insert malicious records that redirect queries to attacker-controlled systems. The purpose of this task is to investigate whether entries placed in the Additional section of a spoofed DNS reply will be cached by the local DNS server.

###  Attack Strategy & Verification 

When responding to a query for `www.example.com`, construct a forged DNS reply that includes the following entries, in addition to the normal Answer section:
```bash 
;; AUTHORITY SECTION:
example.com.        259200   IN   NS   ns.attacker32.com.
example.com.        259200   IN   NS   ns.example.com.

;; ADDITIONAL SECTION:
ns.attacker32.com.  259200   IN   A    1.2.3.4      (1)
ns.example.net.     259200   IN   A    5.6.7.8      (2)
www.facebook.com.   259200   IN   A    3.4.5.6      (3)
```

Entries (1) and (2) correspond to hostnames that appear in the Authority section and therefore serve as glue records. Entry (3) is unrelated to any name in the response; it simply provides an unsolicited IP address for `www.facebook.com`.

Use Scapy to craft and send this spoofed DNS reply. After launching the attack, examine the DNS cache on the local DNS server and determine:
- Which entries from the Additional section were successfully cached
- Which entries were not cached
- Why the server behaved this way

## Guideline
Several tasks in this lab require you to use Scapy to sniff DNS queries and craft spoofed DNS responses. A complete example demonstrating how to capture a DNS query and generate a forged DNS reply is already provided in the `Labsetup.zip` file (inside the `volumes` folder). This sample code shows how to construct a DNS packet containing one record in the Answer section, two records in the Authority section, and two records in the Additional section. You should refer to this example as a template when implementing your own spoofing logic throughout the lab.

## Potential Issue: Slow Packet Sniffing and Spoofing in Containers

During this lab, you may encounter a performance issue when running the environment using containers (e.g., Docker). In some cases, the packet sniffing and spoofing operations inside containers can be slower than expected. As a result, the spoofed DNS response generated by the attacker may arrive later than the legitimate response from the Internet, even though the attacker is on the same local network as the victim.

### Workaround 
If this situation occurs, a practical workaround is to artificially slow down outbound traffic to the external network. By introducing a small delay for outgoing packets, the legitimate DNS responses from the Internet will take longer to reach the local network. This increases the chances that the attacker’s spoofed response will arrive first.

This delay can be introduced using the Linux `tc` (traffic control) command on the router container.

The router in this lab environment has two network interfaces: `eth0` and `eth1`.

Make sure to apply the delay on the interface connected to the external network (`10.8.0.0/24`).

#### Add Network Delay

The following command introduces a 100 ms delay to outgoing network traffic:

```bash 
# Delay outgoing traffic by 100 ms
tc qdisc add dev eth0 root netem delay 100ms
```

#### Remove the Delay Rule
If you need to remove the delay rule, run:
```bsh 
tc qdisc del dev eth0 root netem
```

#### Check Current Traffic Control Rules

To view all traffic control rules currently applied to the interface:
```bash 
tc qdisc show dev eth0
```

### Recommendation
You may keep this delay rule active for the entire duration of the lab, as similar timing issues may occur in multiple attack tasks. The added delay helps ensure that spoofed packets generated by the attacker have a higher chance of arriving before legitimate DNS responses.

## Grading

- Post your report in <a href="https://cs.ycp.edu/marmoset" target="_blank">Marmoset</a> by the scheduled due date in the syllabus. Your grade for this lab will be composed of:
- You need to submit a detailed lab report, with **screenshots**, to describe what you have done and what you have observed. You also need to provide explanation to the observations that are interesting or surprising. Please also list the important code snippets followed by explanation. Simply attaching code without any explanation will not receive credits.
  - 30% - Design:  The overall quality and structure of your exploit.
  - 30% - Observations: Insightfulness and depth in your understanding of the task
  - 40% - Explanation: Clarity and completeness of your report and analysis.
  - *Extra Credit*: Additional investigation beyond the requirements.

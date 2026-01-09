---
layout: default
course_number: CS335
title: Resources
---

This page contains links and tools to help you throughout the course.

Development Environment
------------------------------------
- Microsoft  [Visual Studio](https://visualstudio.microsoft.com/) is an integrated development environment from Microsoft. It is used to develop computer programs, as well as websites, web apps, web services and mobile apps.
- [GCC](https://gcc.gnu.org/), the GNU Compiler Collection.
- [GDB](https://www.gnu.org/software/gdb/), the GNU Project Debugger.
  - A [tutorial](https://visualgdb.com/gdbreference/commands/) by the _visualgdb_ folks.
- [GNU make](http://www.gnu.org/software/make/) and GNU make [manual](http://www.gnu.org/software/make/manual/make.html).

Passive Capture
------------------------------------
- [Wireshark](https://www.wireshark.org/) is the most popular tool for passive packet capture and analysis.
    - [Sample Captures](https://gitlab.com/wireshark/wireshark/-/wikis/SampleCaptures)
- [Microsoft Message Analyzer](https://docs.microsoft.com/en-us/message-analyzer/microsoft-message-analyzer-operating-guide) is an extensible tool for analyzing network.
traffic on Windows. *Retired on November 25, 2019.*
- [TCPDump & libpcap](http://www.tcpdump.org/) is a powerful command-line packet analyzer; and libpcap, a portable C/C++ library for network traffic capture, and it's windows implementation [WinPcap](https://www.winpcap.org/).
  - Programming with [pcap](http://www.tcpdump.org/pcap.htm) by Tim Carstens.
- [netwox](http://www.cis.syr.edu/~wedu/Teaching/cis758/netw522/netwox-doc_html/) is a **netw**ork toolb**ox** is a network _toolbox_ for network administrators and network hackers.
- [netwib](http://www.cis.syr.edu/~wedu/seed/Labs/Lab_Setup/netw522/) is a <b>netw</b>ork l<b>ib</b>rary is a network _library_, for network administrators and network hackers.
  - netwox already contains several tools using the netwib network library.
- [netwag](http://www.cis.syr.edu/~wedu/seed/Labs/Lab_Setup/netw522/netwag-doc_html/) is a graphical front end for netwox.
  - netwox & netwag [Troubleshooting guide](docs\netwox.pdf).
- [NetworkMiner](https://www.netresec.com/?page=NetworkMiner) is an open source Network Forensic Analysis Tool.
- [tcptrace](http://www.tcptrace.org/) is a tool for analysis of TCP dump files.
- [tcpflow](https://github.com/simsong/tcpflow) is a powerful command line based tool for analyzing network traffic. 

Active Capture
------------------------------------
- The [Canape tool](https://github.com/ctxis/canape) as a generic network protocol man-in-the-middle testing, analyzing, and exploitation tool with a usable GUI.
- [Canape Core](https://github.com/tyranid/CANAPE.Core/releases/) is a stripped-down fork of the original Canape code base, are designed for use from the command line.
- [Mallory](https://github.com/intrepidusgroup/mallory/) is an extensible man-in-the-middle tool that acts as a network gateway, which
makes the process of capturing, analyzing, and modifying traffic transparent to the application being tested.

Network Connectivity
------------------------------------
- The [Hping](http://www.hping.org/) tool is similar to the traditional ping utility, but it supports more than just ICMP echo requests.
- [Netcat](http://netcat.sourceforge.net/) is a command line tool that connects to an arbitrary TCP or UDP port and allows you to send and receive data.
  - Netcat cheat sheet [pocket reference guide](docs/netcat_cheat_sheet_v1.pdf).
- If you need to scan the open network interface on a remote system, nothing is better than [Nmap](https://nmap.org/).
- _netdiscover_ is a network address discovering tool, install by using `sudo apt-get install netdiscover`.
- [Masscan](https://github.com/robertdavidgraham/masscan/) is the world's fastest internet port scanner.
- [curl](https://curl.se/) is a command line tool and library for transferring data with URLs (since 1998).
- [wget](https://www.gnu.org/software/wget/) is a free software package for retrieving files using HTTP, HTTPS, FTP and FTPS.
- [HTTPie—aitch-tee-tee-pie](https://httpie.io/) is a user-friendly command-line HTTP client for the API era.

Web Testing (Web Proxies)
------------------------------------
- [Burp or Burp Suite](https://portswigger.net/burp) is the gold standard of commercial web application–testing tools.
- OWASP Zed Attack Proxy [ZAP](https://www.zaproxy.org/).
- [w3af](http://w3af.org/) is a Web Application Attack and Audit Framework.
- [mitmproxy](https://mitmproxy.org/) is an interactive, SSL/TLS-capable intercepting proxy with a console interface for HTTP/1, HTTP/2, and WebSockets.
- [Charles](https://www.charlesproxy.com/) is an HTTP proxy, an HTTP.
monitor, and a reverse proxy. It enables you to view HTTPS traffic.
- [HTTrack](https://www.httrack.com/) website copier.

Vulnerability Exploitation
------------------------------------
- [Metasploit](https://github.com/rapid7/metasploit-framework) is pretty much the only game in town when you need a generic vulnerability exploitation framework, at least if you don’t want to pay for one.
- [Ettercap](https://www.ettercap-project.org/) is a comprehensive suite for man in the middle attacks.
- [Scapy](https://scapy.net/) is a network packet generation and manipulation library for Python.
  - [Scapy Usage](https://scapy.readthedocs.io/en/latest/usage.html)
- [sqlmap](http://sqlmap.org/) allows for easy discovery and exploitation of SQL injection vulnerabilities.
- [sqlninja](https://sqlninja.sourceforge.net/) is a SQL Server injection & takeover tool.
- [ghauri](https://github.com/r0oth3x49/ghauri) utomates the process of detecting and exploiting SQL injection security flaws.
- [nessus](https://www.tenable.com/products/nessus/nessus-essentials) is a proprietary vulnerability scanner developed by Tenable Network Security.
- [Nikto](https://cirt.net/Nikto2) is an Open Source web server scanner.
- [OpenVAS](http://www.openvas.org/) is an Open Vulnerability Assessment Scanner.
- [Amass](https://github.com/owasp-amass/amass) in-depth attack surface mapping and asset discovery.
- [Gobuster](https://github.com/OJ/gobuster) is a tool used to brute-force URIs including directories and files as well as DNS subdomains.
- [dsniff](https://www.monkey.org/~dugsong/dsniff/) is a collection of tools for network auditing and penetration testing.
- [Arachni](https://ecsypno.com/pages/arachni-web-application-security-scanner-framework) is an open source, modular, high-performance Ruby framework with focus on evaluating the security of web applications.

Network Spoofing
------------------------------------
- [Ettercap](https://www.ettercap-project.org/) is a man-in-the-middle tool designed to listen to network traffic between two devices.
- [DNSMasq](https://thekelleys.org.uk/dnsmasq/doc.html) is designed to quickly set up basic network services, such as DNS and DHCP, so you don’t have to hassle with complex service configuration.
- [arpspoof](https://github.com/alandau/arpspoof) is a simple ARP spoofer for Windows.

Password Crackers
------------------------------------
- [Hash Suite](https://hashsuite.openwall.net/) is a Windows program to test security of password hashes.
- [hashcat](https://hashcat.net/hashcat/) is the world's fastest and most advanced password recovery utility.
- [John the Ripper](https://www.openwall.com/john/) is a fast Open Source password cracker.
  - The official [repo](https://github.com/magnumripper/JohnTheRipper/).
  - The [password hash file](http://contest-2012.korelogic.com/) bundle from the KoreLogic 2012 DEFCON challenge. Decompress with ```bzip2 -dk cmiyc_2012_password_hash_files.tar.bz2```.
  - A large word list containing 1,493,677,782 words can be found at [crackstation](https://crackstation.net/crackstation-wordlist-password-cracking-dictionary.htm).
  - Bruteforce find [wordlists](https://weakpass.com/wordlist)
- [RainbowCrack](http://project-rainbowcrack.com/index.htm) crack hashes with rainbow tables.
- [Ophcrack](https://ophcrack.sourceforge.io/) is a free Windows password cracker based on rainbow tables.
- [THC-Hydra](https://github.com/vanhauser-thc/thc-hydra) is one of the most widely used _online_ hacking tool.
- [aircrack-ng](https://www.aircrack-ng.org/) is probably the best Wi-Fi hacking software available.
- Forgot your old windows box password? Try [ntpasswd](http://pogostick.net/~pnh/ntpasswd/).

Steganography/Content Discovery
------------------------------------
- [OpenStego](https://www.openstego.com/) can hide any data within a cover file (e.g. images).
- [Camouflage](http://camouflage.unfiction.com/) allows any file to be hidden within any other file.
- [KiteRunner](https://github.com/assetnote/kiterunner) Contextual Content Discovery Tool.

System Utilities
------------------------------------
- [Sysinternals Suite](https://docs.microsoft.com/en-us/sysinternals/) is a suite of more than 70 freeware utilities used to monitor, manage and troubleshoot the Windows operating system.
- [NirSoft](https://www.nirsoft.net/) is a unique collection of small and useful utilities, including password recovery, networking tools, forensics and more.

API Development
------------------------------------
- [Postman](https://www.getpostman.com/) API development environment.
- [Fiddler](https://www.telerik.com/fiddler) is free web debugging proxy for any browser, system or platform.
- [A RESTful Tutorial](https://www.restapitutorial.com/).

Forensics
------------------------------------
- [Open Source Digital Forensics](https://www.sleuthkit.org/).
- [OfflineRegistryView](https://www.nirsoft.net/utils/offline_registry_view.html) is a simple tool for Windows that allows you to read offline Registry files from external drive and view the desired Registry key in _.reg_ file format. OfflineRegistryView is by [NirSoft](https://www.nirsoft.net/) which also provides a plethora of other related tools.
- [sleuthkit.org](https://www.sleuthkit.org/) is the official website for The Sleuth Kit®, Autopsy®, and other open source digital investigation tools.  
- The Cyber Swiss Army Knife [CyberChef](https://gchq.github.io/CyberChef) - a web app for encryption, encoding, compression and data analysis.
- Hash, hashing and encryption toolkit: [https://md5hashing.net/](https://md5hashing.net/)
- [Maltego](https://www.maltego.com/) is a computer forensics and OSINT tool.

Reverse Engineering
------------------------------------
- The Java Decompiler [JD](http://java-decompiler.github.io/) aims to develop tools in order to decompile and analyze Java 5 “byte code” and the later versions.
- [IDA PRO](https://hex-rays.com/) is the best-known tool for reverse engineering executables
- [Hoppper](https://www.hopperapp.com/) is the macOS and Linux  Disassembler.
- [ILSpy](https://github.com/icsharpcode/ILSpy) is the open-source .NET assembly browser and decompiler.
- [.NET Reflector](https://www.red-gate.com/products/dotnet-development/reflector/) is the original .NET decompiler.
- [OllyDbg](http://www.ollydbg.de/) is a 32-bit assembler level analyzing debugger for Microsoft Windows.
- [Ghidra](https://ghidra-sre.org/) software reverse engineering (SRE) suite of tools developed by NSA's Research Directorate in support of the Cybersecurity mission.

Open-Source Intelligence
------------------------------------
- [OSINT Framework](https://osintframework.com/) provides holistic look at resources and techniques.
- [https://osintcurio.us/](https://osintcurio.us/).
- [OSINT framework](https://start.me/p/ZME8nR/osint) by Bruno Mortier.
- [Shodan](https://www.shodan.io/) is the first search engine for the Internet of Everything.
- Flight & Vessel tracking
  - [https://www.adsbexchange.com](https://www.adsbexchange.com)
  - [https://opensky-network.org](https://opensky-network.org)
  - [https://flightaware.com](https://flightaware.com)
  - [http://shipfinder.co](http://shipfinder.co)
  - [http://www.myshiptracking.com](http://www.myshiptracking.com)
  - [https://www.marinetraffic.com](https://www.marinetraffic.com)
  - [https://www.vesselfinder.com](https://www.vesselfinder.com)
  - [https://www.flightradar24.com](https://www.flightradar24.com)
  - [https://planefinder.net](https://planefinder.net)
- [DNSDumpster](https://dnsdumpster.com/) DNS recon & research, find & lookup DNS records.
- [spiderfoot](https://www.spiderfoot.net/) automates OSINT so you can find what matters, fast.
- [PimEyes](https://pimeyes.com/en) is a face/reverse image search engine.
- [OctoSuite](https://pypi.org/project/octosuite/) is a framework for gathering OSINT on GitHub users, repositories and organizations.
- [Recon-Ng](https://github.com/lanmaster53/recon-ng) is a gathering tool aimed at reducing the time spent harvesting information from open sources.
- [theHarvester](https://github.com/laramies/theharvester) gathers names, subdomains, IP, email and URLs.
- [metagoofil](https://code.google.com/archive/p/metagoofil/) metadata information (pdf,doc,xls,ppt,docx,pptx,xlsx) gathering. 
- [searchcode](https://searchcode.com/) searches 75 billion lines of code from 40 million projects.

Supplemental Material
------------------------------------
- [High Performance Browser Networking](https://hpbn.co/) is what every web developer should know about networking and web performance.
- [Eloquent JavaScript](https://eloquentjavascript.net/) is a book about JavaScript, programming, and the wonders of the digital.
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/#structs).
- [Principles of chaos engineering](https://principlesofchaos.org/)
- [Programmable Web](https://www.programmableweb.com/) - retired. 

Security Threats
------------------------------------
-  The [OWASP Top Ten Project](https://www.owasp.org/index.php/Category:OWASP_Top_Ten_Project) is a powerful awareness document for web application security.
-  The [OWASP Cheat Sheet Series](https://cheatsheetseries.owasp.org/) was created to provide a concise collection of high value information on specific application security topics.
- [CWE™](https://cwe.mitre.org/compatible/product.html) is a community-developed list of common software security weaknesses.
- [Web Security Threat Classiﬁcation](http://www.webappsec.org/projects/threat/).
- [The CAPTCHA Project](http://www.captcha.net/) Telling Humans and Computers Apart.
- [CVE Details](https://www.cvedetails.com/) the ultimate security vulnerability data source.
- [Common Vulnerability Scoring System Calculator](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator)
- [OWASP Threat Modeling Cheat Sheet](https://cheatsheetseries.owasp.org/cheatsheets/Threat_Modeling_Cheat_Sheet.html)

Bounty Platform
-----------------------------------
- [HackerOne](https://www.hackerone.com/) is a vulnerability coordination and bug bounty platform that connects businesses with cybersecurity researchers.
- [Bugcrowd](https://www.bugcrowd.com/) is a crowdsourced security platform; managing organizations bug bounty, vulnerability disclosure, and next-gen pen test programs.

Penetration Testing Platforms
-----------------------------------
- [Hack The Box](https://www.hackthebox.eu/) is an online platform to test and advance your skills in penetration testing and cyber security.
- [Vulnhub](https://www.vulnhub.com/) provides materials that allows anyone to gain practical 'hands-on' experience in digital security.
- [Hack This Site](https://www.hackthissite.org/) is a legal free training ground for users to test and expand their hacking skills.
- The [wargames](https://overthewire.org/wargames/) offered by the OverTheWire community can help you to learn and practice security concepts in the form of fun-filled games
- [We Chall](http://www.wechall.net) offers computer-related problems and links to other challenges.
- [Google Gruyere](https://google-gruyere.appspot.com/) - Web Application Exploits and Defenses.
- [247CTF](https://247ctf.com/) The game never stops.
- [TryHackMe](https://tryhackme.com/)

Repositories
-----------------------------------
 - [BIND 9](https://gitlab.isc.org/isc-projects/bind9) source code and issues.
 - [Google Project Zero](https://github.com/googleprojectzero) make the discovery and exploitation of security vulnerabilities more difficult, and to significantly improve the safety and security of the Internet for everyone.

Exploit Databases
-----------------------------------
- The [Exploit Database](https://www.exploit-db.com/) is a CVE compliant archive of public exploits and corresponding vulnerable software, developed for use by penetration testers and vulnerability researchers.
  - The [Google Hacking Database](https://www.exploit-db.com/google-hacking-database) is a project from the Exploit-DB.com domain name, an alternative way to find vulnerable apps and extract information or to gain privileges.
- [CXSecurity](https://cxsecurity.com/) database offers direct access to latest exploits from a web-based interface.
- [Rapid7](https://opendata.rapid7.com/) repository of vetted computer software exploits and exploitable vulnerabilities.
- [Vulnerability Lab](https://www.vulnerability-lab.com/) offers access to a large vulnerability database complete with exploits and PoCs for research purposes.

Technical Documentation
-----------------------------------

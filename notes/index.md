---
layout: default
course_number: CS335
title: "Random Notes & Code Snippets"
---

This page contains links to notes and code snippets.

- [Upgrade VM](#upgrade-vm)
- [Setup Environment](#setup-environment)
- [File Commands and Directory Navigation](#file-commands-and-directory-navigation)
- [Permissions](#permissions)
- [Networking](#networking)
- [Process commands](#process-commands)
- [Clone a website](#clone-a-website)

#### Upgrade VM 
- ```sudo apt update``` - downloads package information from all configured sources.
- ```sudo apt upgrade``` - will upgrade all installed packages to their latest versions.
- ```sudo apt-get autoremove``` - deletes orphaned packages, or dependencies that remain installed after you have installed an application and then deleted it.
- ```sudo apt-get clean``` - removes all packages from the cache.

<a href="#">To top</a>

#### Setup Environment
- Sublime: ```sudo snap install sublime-text --classic```  
- Visual Studio Code: ```sudo snap install --classic code```
- clion: ```sudo snap install clion --classic```
   
<a href="#">To top</a>

#### File Commands and Directory Navigation

- ```cd``` go to _$HOME_ directory.
- ```cd ...``` go one level up the directory tree.
- ```cd /etc``` to change to the _/etc_ directory.  
- ```ls``` list all files.
  - Use ```-R``` to list all-subdirectories as well
  - ```-a``` will list hidden files as well
  - Use the ```-al``` argument to view details
- ```pwd``` lists the present working directory.
- ```mkdir directory``` created a _directory_.
- ```rm -r directory``` removes the _directory_ and its contents recursively. Use the ```f``` argument to forcefully remove, re: ```rm -rf directory```.
- ```touch file``` will create an empty _file_.
- ```rm file``` removes a _flle_.
- ```cp file file2``` will copy _file_ to _file2_.
- ```mv file file2``` renames or moves _file_ to _file2_.
- ```cat filename``` will display the contests of _filename_.
- ```cat > filename```  creates a new file with _filename_.
  
<a href="#">To top</a>

#### Permissions

![](images/file_permissions.png "File Permissions")

- Legend
  - User or Owner ```U```
  - Group ```G```
  - World (Other Users) ```W```
- Permission Classes
  - Read ```r```
  - Write ```w```
  - Execute ```x```
- File Type
  - ```-``` regular file
  - ```d``` directory
- Examples
  - file _desktop.ini_: ```-rwxrwxrwx 1 seed seed 282 Dec 27 10:10 desktop.ini```
  - directory _host_: ```drwxrwxrwx 1 seed seed 4096 Jan 20 13:22 host```

  | Number | Permission Type       | Symbol |
  | ------ | --------------------- | ------ |
  | 0      | No Permission         | ---    |
  | 1      | Execute               | --x    |
  | 2      | Write                 | -w-    |
  | 3      | Execute + Write       | -wx    |
  | 4      | Read                  | r--    |
  | 5      | Read + Execute        | r-x    |
  | 6      | Read +Write           | rw-    |
  | 7      | Read + Write +Execute | rwx    |
- Permission Examples  
  - ```chmod 777 filename```: _rwx rwx rwx_
  - ```chmod 775 filename```: _rwx rwx r-x_
  - ```chmod 755 filename```: _rwx r-x r-x_
  - ```chmod 664 filename```: _rw- rw- r--_
  - ```chmod 644 filename```: _rw- r-- r--_

<a href="#">To top</a>

#### Networking

- ```ifconfig -a``` displays all network interfaces and IP address.
- ```hostname -I``` displays the IP addresses of the host (all local IP addresses).
- ```host domain``` displays IP address for _domain_.
- ```ping host``` sends ICMP echo request to _host_.
- ```whois domain``` displays whois records for _domain_.
- ```dig domain``` displays DNS information for _domain_.
- ```dig -x IP``` does reverse lookup of _IP_ address.
- ```nslookup``` query Internet name servers interactively.
- To display the IP/kernel routing table:
  - ```netstat -rn```
  - ```ip route```
  - ```route -n```

<a href="#">To top</a>

#### Process commands

- ```bg``` sends a process to the background.
- ```fg```	runs a stopped process in the foreground.
- ```top``` shows	details on all active processes.
- ```ps```	gives the status of processes running for a user.
- ```pidof```	gives the process id (PID) of a process.
- ```ps PID```	gets the status of a particular process.
- ```kill PID```	kills a process with _PID_ .
- ```nice```	starts a process with a given priority.

<a href="#">To top</a>

#### Clone a website 
```wget --mirror --convert-links --adjust-extension --page-requisites --no-check-certificate --no-parent https://site-to-copy.com```
  - ```--mirror``` make the download recursive.
  - ```--no-parent``` do not crawl the parent/top directory.
  - ```--convert-links``` makes all the links work properly with the offline copy.
  - ```--page-requisites``` download JS/CSS files.
  - ```--adjust-extension``` add the appropriate extensions (e.g. html, css, js) to files.
  - ```--no-check-certificate``` ignores SSL certificate errors
<a href="#">To top</a>

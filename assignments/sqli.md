---
layout: default
course_number: CS335
title: Lab - SQL Injection
---

# Lab: SQL Injection (SQLi)


## Learning Objectives
This lab explores SQL injection vulnerabilities in web applications. Students will exploit vulnerabilities in a provided employee‑management web application and then apply defenses such as prepared statements.

### Topics covered:
- SQL SELECT and UPDATE statements
- SQL injection attacks
- Prepared statements

### References 
- [Bobby Tables](https://bobby-tables.com/): A guide to preventing SQL injection
- [SQL Injection Prevention](https://cheatsheetseries.owasp.org/cheatsheets/SQL_Injection_Prevention_Cheat_Sheet.html)

---

## Lab Environment 
Setup files: [Labsetup.zip](..\notes\sqli\Labsetup.zip)

A vulnerable web application is provided using two Docker containers:
- Web application container: `10.9.0.5`
- Database container: `10.9.0.6`
  - A database `sqllab_users` contains a table credential with employee information.

### User Roles in the Web Application
Employees can view and update their personal information in the database through this web application. There are two main roles:
- **Administrator**: A privileged role with the ability to manage each individual employee’s profile information.
- **Employee**: A normal role that can view or update only his or her own profile.

### DNS Setup
Add the following entry to `/etc/hosts`:

```bash 
10.9.0.5   www.seed-server.com
```

#### Login information

| User  | Password  |
| ----- | --------- |
| admin | seedadmin |
| alice | seedalice |
| boby  | seedboby  |
| ryan  | seedryan  |
| ted   | seedted   |
| samy  | seedsamy  |

---

## Ethical Use Notice
This lab is intended **solely for educational use within a controlled environment**. The techniques demonstrated must **never** be used on systems you do not own or lack explicit authorization to test.

---

## Lab Tasks

### Task 1 — Get Familiar with SQL Statements
The goal of this task is to help you become familiar with the structure and behavior of SQL queries within the provided database. You will explore the schema, inspect tables, and practice writing basic queries. This foundational understanding prepares you for identifying how SQL injection attacks interact with backend logic.

1. Get a shell inside the MySQL container.

2. Log in:
```bash 
mysql -u root -pdees
```
3. Load the database:
```bash 
use sqllab_users;
show tables;
```

Write a SQL query to print all profile information for employee Alice. 

### Task 2 — SQL Injection Attack on SELECT Statement
This task focuses on exploiting vulnerabilities in the login mechanism by manipulating user input to alter the SQL query. The goal is to understand how authentication can be bypassed when user input is not properly sanitized. You will test injection both through the webpage and through command‑line requests.

The login page at:
```bash
http://www.seed-server.com
``` 

is vulnerable to SQL injection. 

The authentication code:

```php
$input_uname = $_GET['username'];
$input_pwd   = $_GET['Password'];
$hashed_pwd  = sha1($input_pwd);

$sql = "SELECT id, name, eid, salary, birth, ssn, address, email, nickname, Password
        FROM credential
        WHERE name='$input_uname' and Password='$hashed_pwd'";
```

#### Task 2.1 — SQL Injection via Webpage
The goal is to craft input that changes the logic of the login query so you can authenticate as a privileged user without knowing their password. This demonstrates how dangerous unsanitized input fields can be. You will observe how the application responds to malicious input.

**Objective**: Log in as **admin** without knowing the password.

You must determine what to enter in the **Username** and **Password** fields to bypass authentication.

### Task 2.2 — SQL Injection via Command Line
This task teaches you how to perform the same injection attack using direct HTTP requests. The goal is to understand how encoding and URL‑based attacks work outside the browser. It reinforces that SQL injection vulnerabilities can be exploited from multiple interfaces.

Use curl to send a crafted GET request.

Example format:

```bash 
curl 'www.seed-server.com/unsafe_home.php?username=alice&Password=11'
```

Notes:
- Encode `'` as `%27`
- Encode spaces as `%20`

Repeat Task 2.1 using only command‑line injection.

#### Task 2.3 — Attempt to Append a Second SQL Statement
Here, the goal is to explore whether the backend allows multiple SQL statements in a single request. You will attempt to append a second statement and observe how the system reacts. This helps you identify built‑in protections that limit the severity of SQL injection attacks.

**Objective**: Try to inject a second SQL statement using `;`.

Example:
Turn one SQL query into two (e.g., an `UPDATE` or `DELETE`).

A countermeasure prevents multiple statements. Identify the countermeasure and describe it in your report.

### Task 3 — SQL Injection Attack on UPDATE Statement
This task shifts focus from authentication to data manipulation. The goal is to understand how injection vulnerabilities in update queries can allow unauthorized changes to stored data. You will explore how attackers can escalate impact beyond simple login bypasses.

The Edit Profile page:

```bash 
http://www.seed-server.com/unsafe_edit.php
``` 

executes:

```php
$hashed_pwd = sha1($input_pwd);
$sql = "UPDATE credential
        SET nickname='$input_nickname',
            email='$input_email',
            address='$input_address',
            Password='$hashed_pwd',
            PhoneNumber='$input_phonenumber'
        WHERE ID=$id;";
```

This is vulnerable to SQL injection.

#### Task 3.1 — Modify Your Own Salary
The goal is to exploit the vulnerable update query to change fields that the interface normally restricts. This demonstrates how injection can override application‑level permissions. You will see how backend logic can be manipulated even when the UI appears secure.

**Objective**: As **Alice**, exploit the vulnerability to change your own salary, even though the UI does not allow it.

#### Task 3.2 — Modify Another Employee's Salary
This task highlights privilege escalation through SQL injection. The goal is to modify another user’s data, even though your role should not allow it. This shows how injection can break role‑based access controls entirely.

**Objective**: Still as Alice, reduce **Boby's** salary to 1 dollar using SQL injection.

#### Task 3.3 — Modify Another Employee's Password
The goal is to change another user’s password by injecting malicious input into the update query. This demonstrates how attackers can take over accounts without knowing the original credentials. You will confirm the attack by logging in as the targeted user.

**Objective**: While still logged in as Alice, change **Boby's** password to a new value of your choice.

Log in as **Boby** using the new password.

**Note**: Passwords are stored as `SHA‑1` hashes.

Multiple ways to generate SHA1 of a password:
- `mysql> select sha1('new_password');`
- [CyberChef](https://gchq.github.io/CyberChef/) and search for SHA1
- `echo -n 'new_password' | openssl sha1`

### Task 4 — Countermeasure: Prepared Statements
This task introduces a key defense against SQL injection. The goal is to rewrite vulnerable code using prepared statements so that user input cannot alter the structure of SQL queries. You will apply secure coding practices to harden the application.

Prepared statements separate code from data and prevent SQL injection.

Example vulnerable code:

```php
$sql = "SELECT name, local, gender
        FROM USER_TABLE
        WHERE id = $id AND password='$pwd'";
$result = $conn->query($sql);
```

Example using prepared statements:

```php
$stmt = $conn->prepare(
    "SELECT name, local, gender
     FROM USER_TABLE
     WHERE id = ? and password = ?"
);
$stmt->bind_param("is", $id, $pwd);
$stmt->execute();
```

#### Task — Fix the Vulnerable Program
The goal is to apply prepared statements to the provided backend file and eliminate injection vulnerabilities. This reinforces the importance of separating data from executable SQL logic. You will test your fix to ensure the application behaves securely.

Navigate to:
```bash
http://www.seed-server.com/defense/
```

The backend file:
```bash
image_www/Code/defense/unsafe.php
```

Modify the SQL query to use prepared statements. Rebuild or restart the container after changes.

---
### Guidelines

Testing Injection Strings
You may test injection syntax directly in MySQL.

Example:

```bash
SELECT * FROM credential
WHERE name='Alice' and password='pwd';
```

Replace values with your injection string to verify syntax before attacking the web app.

## Grading

- Post your report in <a href="https://cs.ycp.edu/marmoset" target="_blank">Marmoset</a> by the scheduled due date in the syllabus. Your grade for this lab will be composed of:
- You need to submit a detailed lab report, with **screenshots**, to describe what you have done and what you have observed. You also need to provide explanation to the observations that are interesting or surprising. Please also list the important code snippets followed by explanation. Simply attaching code without any explanation will not receive credits.
  - 30% - Design:  The overall quality and structure of your exploit.
  - 30% - Observations: Insightfulness and depth in your understanding of the task
  - 40% - Explanation: Clarity and completeness of your report and analysis.
  - *Extra Credit*: Additional investigation beyond the requirements.

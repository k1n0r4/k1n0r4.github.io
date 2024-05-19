+++
title = 'GCC 2024'
date = 2024-05-05T12:45:19+05:30
featured_image = "/images/gcc/gcc_bg.jpeg"
tags = ["GCC"]
+++

Writing about the work trips is my favorite hobby 😉

So well, in february I got the opportunity to attend a camp at Thailand, the land of amazing cuisine 😋

I attended Global Cybersecurity Camp, which was a 5 days event from 19th Feb to 24th Feb, 2024.

Cybersecurity enthusiasts and experts from 11 different Asian Countries 🌎 joined on this venture. The participating countries were Japan, Vietnam, South Korea, Malayasia, Singapore, Indonesia, Taiwan, India, Thailand and Australia. Unfortunately Australia couldn't make it onsite for the event.

This year marked India's first participation in the camp !!!


![GCC](/images/gcc/gcc_1.png)


From India we were 4 students and 2 staff members who participated in the onsite event.
We left on 17th Feb from different parts of India, and reached Thailand around early morning on 18th Feb 2024.

![GCC](/images/gcc/gcc_2.png)

After everyone arrived, we were picked up by one of the Thailand staff member, Gong, and they had arranged a private van for us. The journey from airport to the hotel was 2 hours and we stopped by on our way for breakfast.

That was my first time live seeing the street food of south eastern asian countries, and it was truly mesmerizing😋 

![GCC](/images/gcc/gcc_3.png)

We had to exit the vehicle and walk when we got close to the hotel since there was a marathon going on. Later on, we found out that it was actually a triathlon with competitors from several nations.

![GCC](/images/gcc/gcc_4.jpg)

Team Japan was getting off their van just as we were coming out of ours.

We had a little chat with Team Japan after reaching the hotel and went out for lunch with them at a nearby restaurant.

We had authentic Thai food and specially the delicious Thai soups. 

![GCC](/images/gcc/gcc_5.jpg)


After lunch, we all had an icebreaker session in the evening during which we introduced ourselves and our nations to one other. 

The introductions was pretty cool and the ppts were very chill and casual. I was surprised and at the same time excite to see that people from some famous and top teams had joined us on this camp like Hitcon, hackerscrew and players who have participated in Defcon Finals onsite.
There were poeple who had a CVE on their name already.

Jumping on to Day 1

### Day 1

The opening sessions were given by the organisers and everyone was sitting with their groupmates on a round table. I was in Team 9, and my teammates were from the countries, South Korea, Taiwan, Vietnam, Thailand and Malaysia.

We were also introduced to group projects and our Group porject was a CTF Battle against Group 4.

After a good ethics session we had lunch and then we were onto our workshops.

Our first session was **Active Directory Attack Techniques**. It was taken by [Mars Cheng](https://www.linkedin.com/in/marscheng93/) and it was quite a detailed session with labs. It was honestly a lot to take in, in one time. 

The dinner was followed by a house warming typa party at a hotel room. The people were very welcoming and we played some dares as well. 


### Day 2

Day 2 had 2 sessions.

First one was **Handling Threat Intelligence**: Techniques of consuming and creating threat intelligence by [Tomohisa Ishikawa](https://www.linkedin.com/in/tomohisaishikawa/) [JP]

This session involved identifying threats and malwares using different techniques, one of them being by writing YARA rules. After an intro session, we were provided with a lab where different malicious files were given to us, and we had to write YARA rules depending on the file hash and data that can be retrieved from sites like virustotal about the hash.

After lunch, second session was **Fuzzing in the kernel world** by [Yi-Han Wang](https://www.linkedin.com/in/yi-han-wang-299749208/) [TW]

This was an intro to Fuzzing and Virtualisation followed by the lab for both of these. This was a group work where each group was provided with a hosted VM which could be accessed by all the members of the team. This was a fun activity, and even though the set up took a lot of time, those breaks gave us some good time to talk and enjoy the snacks deployed outside the hall.

### Day 3

**How (not) to build a vulnerable LLM web app** by [Shota Shinogi](https://www.linkedin.com/in/shota-shinogi/) [JP] was held in morning, which was again a group work and had a competitive environment as to which team manages to build a vulnerable LLM web app first. We were not just taught how to build a vulnerable LLM web app but were also challenged to fix that vulnerablility and make a secure version of that app. At the end of the session, one of the teams was asked to present their app on the stage as well. Group 1 presented their app and they had built an undefeatable game.

My team built a code sanitiser meant for performing calculations only.

Second session of the day was a forensics session, **Hunting with a dinosaur** by [Andreas van Leeuwen Flamino](https://www.linkedin.com/in/andreasvanleeuwenflamino/) [NL], where we were introduced to a very powerful DFIR tool [velociraptor](https://www.rapid7.com/products/velociraptor/). We were given a storyline and a situation, along with some questions. We were supposed to perform the investigation using the tool and find answers to all the questions given. We learned about how to search different artefacts, filter them and extract necessary information  from them. It was a fun experience.

### Day 4

Fourth entire day was dedicated to **Hands-on Car Hacking**
By [Kamel Ghali](https://www.linkedin.com/in/mohammad-kamel-ghali/) [JP]. Kamel is incredibly helpful and has a really cool personality. In this group exercise, which was our first hardware-related workshop, we were given pre-made virtual machine images to use as workstations. We had to hack a Bluetooth device in order to get its contents before using the hacked device to send and receive data.

And that was only the start, as a speedometer and hacking into the speed, breaks, indicator lights, and other things were added later.

![GCC](/images/gcc/gcc_6.jpg)

### Day 5

Last day, we only had one session, **Practical Malware Analysis and .NET Reversing** (Based on Real-World Cases) by [Wei-Chieh Chao](https://www.linkedin.com/in/oalieno/) [TW] and [Yi-Hsien Chen](https://www.linkedin.com/in/yi-hsien-chen-05b411138/) [TW]. As a reverse engineer, I really liked the resources and materials they provided for this training. The resources were very well crafted with a perfect learning curve. A pre-built virtual machine image with all the necessary tools and malware was sent to us.
We were exposed to a variety of the most recent malwares, and there was a CTF hosted by them where we had to submit the answers to the questions posed to us during the malare session in a form of flag.

### After work

The same venue where our workshops were conducted allowed us to continue working on our group projects until 10pm at night. This was the time were the entire team interacted and discussed with each other. I thought this was a great opportunity to learn about the work processes that individuals in many countries follow. 

My group project was CTF Challenges, and we had our CTF platform (ctfd) setup on day 1 itself. In total we had 8 challenges in our CTF which was equal to the number of challenges that were provided to us during the battle phase. On thursday (Day 4), we exchanged our challenges and started working on those. It was a 24 hour CTF, and players from other teams were also given the oppotunity to try out the challenges. My team managed to solve all the challenges from Team 4 (our opponent). The CTF challenges made by us were diverse and included challenges from the fields like forensics, misc, cryptography, reverse engineering, web exploitation (which was based on the past CVE of one of my group member) and wifi hacking. We had a cool black wifi router sitting on our table for the same purpose throughtout the event.

![GCC](/images/gcc/gcc_7.jpg)

### Party

South Eastern Asian countries appear to have a fairly prevalent tradition of parties where people socialize and drink after a long day. There used to be planned party like this every night, and it was a great opportunity to mingle with other students, speakers, and sponsors who had come along for the event. There were several locations for the parties: occasionally it was a little ramyeon party in a room, sometimes drinks by the pool, and occasionally it was a replica of a housewarming celebration in a room.

In Thailand, a well-known convenience store chain called "7 Eleven" is open around-the-clock. Our hotel was not far from one of these store. Every day when we had late work or ran out of snacks at the party, we would stop by to get some.

![GCC](/images/gcc/gcc_8.jpg)

### GSC - Glocal Snack Camp

On 5th day, we officially named the camp as Global Snack Camp because of the amount of snacks we were given from different countries everyday 😂

Every country presented each other with souvenirs, stickers, badges and snacks.

We offered some Indian snacks to the crowd on the last day, and they were quite fond of the food's explosion of flavor. Even though some Japanese people found it hot, they continued to eat and enjoy it. I discovered that parathas (An Indian Dish) are pretty common in Singapore.

![GCC](/images/gcc/gcc_9.png)

### Closing ceremony

Last day, after lunch we had a closing ceremony. We were awarded with certificates for participating in the event which was followed by group presentations on our projects.

![GCC](/images/gcc/gcc_10.jpg)

We were strictly provided only 5 minutes for the presentation and a panel of judges from different countries was present to rate us on our project. The presentation also demanded a demo.

All the presentations were very impressive, and I wonder if given more time, the result would have been phonemonen. There were total 4 project topics which were divided among 9 teams and winner in each topic was announced first. My team won in the CTF Challenge.

Later top 2 teams were selected which were awarded with off sec playground subscriptions for each member. In this, my team (team 9) clutched second position.

![GCC](/images/gcc/gcc_11.jpg)

### After event

The formal GCC event came to an end at this point. Now that we had some spare time, we explored the nearby walkways and beach. Our meal was at a seaside restaurant that was ten minutes away. Every table had a different spicy dish that we dared each other to try.

![GCC](/images/gcc/gcc_12.jpg)

On the last day of the trip, everyone was dropped off at the airport and given a tour by the organizers. The event was really well-planned and executed throughout. We also made a shopping trip at a nearby market, where the main attraction was their well-known snack, dried sea food.

![GCC](/images/gcc/seafoodsnacks.jpg)


We came to a sightseeing spot at the end, took one more picture, and said each other farewell.

Overall, the entire trip was very heart-filling and I personally feel I got a very good exposure to the outside world, their work ethics, their mentalities and their perspective on different topics.

Technically and non-technically, I learned a lot from this trip, and look forward to meet these amazing people again soon!!

Until then, bye bye !!!

![GCC](/images/gcc/gcc_13.jpg)


<br>
<br>

-----------------------------------------------------------------
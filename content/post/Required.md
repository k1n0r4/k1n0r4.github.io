+++
title = 'Required : hxpCTF 2023'
date = 2023-03-30T23:17:45+05:30
featured_image = "/images/required/req_bg.jpeg"
tags = ["Writeup", "Web", "hxpCTF"]
+++

## Description

I have written a super safe flag encryptor. I’m sure nobody can figure out what my original flag was:

0xd19ee193b461fd8d1452e7659acb1f47dc3ed445c8eb4ff191b1abfa7969

<b>Points</b> - 182 points

<b>Solves</b> - 46 solves

<b>CTF</b> Rating - 100

<hr>

## Solution

### Initial Analysis

A total of 300 javascript files are provided with a number of missing files.
In most cases, the files contain a variety of constraints that our original flag bytes must navigate to produce the output mentioned in the description.

Additional to these files is another javascript file named `required.js`.

As can be seen from this file, it reads a flag file first, and then uses a require() function to <b>import modules</b>, which are the javascript files above. These javascript files are imported with three parameters.

### Working of the challenge

By observation we were able to point out few files which were imported repetatively in `required.js`.
Those files are : "555.js", "157.js", "28.js", "299.js", "394.js" and "736.js".

We examined each file one by one.

For example, if we look into `736.js`

```javascript
module.exports=(i,j,t)=>(i+=[],j+"",t=(t+{}).split("[")[0],o={},Object.entries(require('./289')(j,i)).forEach(([K,V])=>Object.entries(V).forEach(([k,v])=>(o[K]=o[K]||{},o[K][k]=v))),require(`./${j}`))
```

This, in turn, calls another module `289.js` and the second (j%30) and first (i%30) parameters are passed through (in the same order) as the parameters for `289.js`.

`Because the flag length is presumably 30, the original 3 parameters i,j and k undergo an operation "%=30" - we will see in the next part of this article why we consider the flag length.` 

Let's see what's inside `289.js`

```javascript
module.exports=(i,j,t)=>(i+=[],j+"",t=(t+{}).split("[")[0],JSON.parse(`{"__proto__":{"data":{"name":"./${i}","exports":{".": "./${j}.js"}},"path": "./"}}`))
```

It simply indicates that the second parameter is executed instead of the first parameter when the first parameter is called. So that's basically one file is being mapped to another file. This mapping is saved in some sorts of cache.

Hence, going through all those 6 repetitive files, we find the correct mappings.

```
"555": (2,3)
"157": (1,3)
"28":  (1,2)
"299": (3,2)
"394": (3,1)
"736": (2,1)
```

There are a few more special files, like `37.js` which is imported at the end with 0 parameters. 


```javascript
// 37.js

module.exports=(i,j,t)=>(i+=[],j+"",t=(t+{}).split("[")[0],console.log(require('./314')(i)))
```

```javascript
// 314.js

module.exports=(i,j,t)=>(i+=[],j+"",t=(t+{}).split("[")[0],s='0x',i.split(',').forEach(b=>(s+=('0'+(b-0).toString(16)).slice(-2))),s)
```

It's clearly visible that `37.js` imports `314.js` which prints out a hex value. This looks similar to the output hex string provided to us in the Description.

Apart from that, there is a module that clears out the current cache.

```javascript
// 556.js

module.exports=(i,j,t)=>(i+=[],j+"",t=(t+{}).split("[")[0],Object.keys(require.cache).forEach(i=>{delete require.cache[i]}))
```

Rest of the files apply constraints like

```javascript
// 490.js

module.exports=(i,j,t)=>(i%=30,j%=30,t%=30,i+=[],j+"",t=(t+{}).split("[")[0],f[i]/=f[29],f[i]&=0xff)
```

`f[i]/=f[29],f[i]&=0xff)` are the constraints where i is being replaced by the first parameter(i) % 30. 
since our flag bytes are read in array name 'f', the indices of 'f' are limited to 30 (i%30) which seems to be the length of the flag.

Our next step is to find out which constraints are manipulating the flag bytes.

### Strategy

To obtain those constraints, we use a simple yet smart approach.Except for some special files mentioned above, all other files are executed directly, which seems to have just the required constraints

In an attempt to print these constraints, a 'console.log' was appended to each file that contained constraints, skipping special files or files not present. Also, to get the correct index numbers in the constraint, we replace those variables with the actual values by appending another line `replaceAll`.

### Extracting the constraints


#### Script #1

```python
import re

some_dict = {
    "555": (2,3),
    "157": (1,3),
    "28":  (1,2),
    "299": (3,2),
    "394": (3,1),
    "736": (2,1)
}

def addcons():

    for i in range(1, 1000):
        if str(i) in some_dict or i == 37 or i == 314:
            continue
        try:
            with open(f'{i}.js', 'r') as f:
                data = f.read()
            if "JSON" in data or "delete" in data:
                continue
            eqn = data[77:-1]
            data = data.split(",")
            data = data[:len(data)-1] + [f'console.log("{eqn}".replaceAll("i", i).replaceAll("j", j).replaceAll("t", t))'] + [data[-1]]
            data = ",".join(data) 
            with open(f'{i}.js', 'w') as f:
                f.write(data)
        except FileNotFoundError:
            continue


def main():
    addcons()

main()

```

<br>

This results in

```java
// 490.js

module.exports=(i,j,t)=>(i%=30,j%=30,t%=30,i+=[],j+"",t=(t+{}).split("[")[0],f[i]/=f[29],console.log("f[i]/=f[29],f[i]&=0xff".replaceAll("i", i).replaceAll("j", j).replaceAll("t", t)),f[i]&=0xff)
```

Once the javascript files have been updated, the required.js file is run with a sample flag. When required.js is invoked, it prints out the constraints due to the presence of the console.log file.


### Final Step

Hereafter, we utilize z3 in order to solve the constraints and calculate the original flag bytes.


#### Script #2

```python
from z3 import *


f = [BitVec(f'flag_{i}', 32) for i in range(30)]

sol = Solver()


sol.add(f[0] == ord('h'))
sol.add(f[1] == ord('x'))
sol.add(f[2] == ord('p'))
sol.add(f[3] == ord('{'))
sol.add(f[29] == ord('}'))


for i in range(30):
    sol.add(f[i] > 32, f[i] < 127)


f[17]+=f[5]
f[17]&=0xff
.
.
.
f[15]-=f[17]
f[15]&=0xff
f[0]=~f[0]&0xff


final = bytes.fromhex("d19ee193b461fd8d1452e7659acb1f47dc3ed445c8eb4ff191b1abfa7969")

for i in range(30):
    sol.add(f[i] == final[i])

print(sol.check())

if sol.check()==sat:
    m = sol.model()

    res = {}
    for j, i in enumerate(m):
        res[str(i)]=int(str(m[i]))
    rkey = list(res.keys())
    rkey.sort()
    sres = {i: res[i] for i in rkey}
    flag = [sres[i] for i in sres]
    print(''.join([chr(i&0xff) for i in flag]))
else:
    print('unsat')

```

Find the complete script [here](/files/required/final_script.py)

### Conclusion

Using a simple trick saves us from emulating or understanding the entire functionality of required.js. 


#### Flag  

<b>`hxp{Cann0t_f1nd_m0dule_'fl4g'}`</b>

<br>
<br>

-----------------------------------------------------------------
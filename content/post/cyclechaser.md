+++
title = 'Cyclechaser : Kalmar CTF 2023'
date = 2023-03-07T14:52:41+05:30
featured_image = "/images/cyclechaser/chaser_bg.png"
tags = ["Writeup"]
+++


I played Kalmar CTF with Team Shakti last weekend and came across this amazing reversing challenge. This challenge utilises functions like `srand()` and `getrandom()`. 

Let's take a look how these functions play a role in this challenge and how I reversed the binary to clutch the flag. There were some calculated hunches that I had to make to reach the correct solution and pretty little pwntools script 🙂.

### Triage

Let's first analyze the challenge [binary](/files/cyclechaser/cyclechaser) statically.

The process generates a 4 byte number via the function `getrandom()` and this random number is then used as the seed for `srand()` function. 

The `getrandom()` function is then used second time to generate 1024 random bytes.

Next there is an attempt to open flag.txt which is already present in the server and we aim to recover the contents of this file.

The content of flag.txt is checked against the flag format and the rest of the flag is stored.

![cyclechaser](/images/cyclechaser/chaser1.png)

Thereafter the program also takes our input which is 16393 bytes long.

<br>

### Initial Approach

Since a lot of things are going on in this challenge, it was difficult to point out as to what approach would be best to take.

Executing the challenge prints out the seed used for srand function, this made me believe that whatever function or part of code is using `rand()` function might be of high importance.

![cyclechaser](/images/cyclechaser/chaser2.png)


In this section we have got a really large `for` loop which runs 131135 times.

The final output that we get is v17 which is changed according to the value of v5 variable, which is in turn being changed based on the condition `if ( j > 0x1FFFF )`

My initial approach was to extract all the rand() values based on the seed and then somehow try to get the flag bytes which are originally v15 in this section. 

Let's make the decompilation more clearer for better understanding.

![cyclechaser](/images/cyclechaser/chaser3.png)

The else part of the above conditional statement involves those random bytes which are generated via the `getrandom()` function, hence those can't be predicted easily.

So that makes it kind of tedious to determine the output if the else section is executed.

### Right Hunch

This made me realise that what if stopping the code flow from entering the `else` part is the primary task that I need to do ?

This is where the role of step function comes in.

![cyclechaser](/images/cyclechaser/chaser4.png)


Until now I haven't checked out step function properly but just skimmed over.

Here v4 needs to be 1 in order to the function to return true, which is when the output will be changed based on the v5 calculated earlier in the previous loop.

In order to make v4 1 we need the first byte of our input to be an odd byte.

But our trouble doesn't ends here, if we look further we could see that our input is being shifted.

That means for everytime the step function is called, the first byte of our input is different.

The shifting that is taking place here is left shift by one byte along with few more calculations which I actually didn't bother to reverse right now.

So if we go back to the main function, I get a quick idea that if the step function returns 0 131072 times and then returns 1 for the rest of the iterations, we might get the flag bytes as it is printed out.

![cyclechaser](/images/cyclechaser/chaser5.png)

Since output is initialized to be zero and continues to remain zero until the iteration number reaches 131072, v5 is directly assigned with the flag bytes which are xorred with the output bytes itself hence giving out the flag itself.

But now we need to figure out what input will make this happen.

I tried analysing the output of the same function with different inputs to find any pattern if it exists.

The pattern that I observed was this -

```c 
#include <stdlib.h>
#include <stdio.h>

int step(char *a1, int a2) {
  char v3 = 0;
  unsigned int v4 ;
  int v8 = 0;
  unsigned int v9 = 0;
  uint i;
  int j;

    v4 = *a1 & 1;
  if (v4) {
    v8 = 1;
    for (i = 0; i < a2; ++i) {
      v9 = v8 + 3 * a1[i];
      v8 = v9 >> 8;
      a1[i] = v9;
    }
    if (v8) {
      puts("Error.");
      exit(99);
    }
  }

  for (j = a2 - 1; j >= 0; --j) {
    char v5 = a1[j] & 1;
    a1[j] >>= 1;
    a1[j] |= v3 << 7;
    v3 = v5;
  }

  if (v3)
    printf("error\n");
  return v4;
}

int main()
{
    char inp[] = "\x00\x00\x00\x00\xaa\xaa\xaa\xaa";
    for (int i = 0; i < 70; ++i)
    {
        printf("%d ", step(inp, 10));
    }
    return 0;
}
```

This returns - 


![cyclechaser](/images/cyclechaser/chaser6.png)

For every null byte as in input, it returns '0' 8 times and thus since I provided 4 null bytes, it gave me 32 zeros and then 1's because of '\xaa'

So since we need to pass the step function 131072 times, which is perfectly divisible by 8, that is, 16384, we provide '16384' null bytes and rest some non-null bytes.

#### Solve script

```python
from pwn import *
yy = b'\x00'
zz= b'\xff'
x = yy*16384+zz*9

io = remote('3.123.91.129',13339)
print('seed', io.recvline())
io.sendline(x)

ret = io.recvline()
print(ret)
io.close()
```

This returns - 

![cyclechaser](/images/cyclechaser/chaser7.png)



Which forms the flag - 

```python=
f='69 5F 73 74 69 6C 6C 5F 68 61 76 65 5F 74 6F 5F 63 6F 6E 73 74 72 75 63 74 5F 61 5F 66 6C 61 67 5F 6F 66 5F 63 6F 72 72 65 63 74 5F 6C 65 6E 67 74 68 5F 66 6F 72 5F 74 68 69 73 5F 63 68 61 6C'.split(' ')
for i in f:
    print(chr(int(i,16)),end='')
```

<b>Output - </b>
i_still_have_to_construct_a_flag_of_correct_length_for_this_chal

#### Flag - 
`kalmar{i_still_have_to_construct_a_flag_of_correct_length_for_this_chal}`
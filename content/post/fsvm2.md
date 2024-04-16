+++
title = 'Fsvm2 : ecscCTF 2024'
date = 2024-04-03T16:30:02+05:30
featured_image = "/images/fsvm2/fsvm2_bg.png"
tags = ["Writeup"]
+++

# fsvm2 - ecscCTF 2024

This is the second part of fsvm, if you haven't checked out the writeup of fsvm yet, then I recommend you to give that a read before reading this. 

In this challenge, a distinct set of VM [binary](/files/fsvm2/vm) and [bytecode](/files/fsvm2/bytecode) are provided with few changes in the instruction set of the VM.

### Initial Triage

With the exception of a few opcodes being implemented, the decompilation of this binary appears almost identical.

16 registers are used by this virtual machine (reg0 - regf). 


I was unable to properly disassemble the emulator after adding the additional opcodes since it appeared to be off by one or two opcodes. It is faster and more efficient to utilize gdb for disassembly since the bytecode is significantly bigger than the previous bytecode.

Let's first see which opcodes are utilized in this instance.


```python!
['(', ')', '*', '+', '-', '/', '4', '5', '6', '7', '8', ';', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'F', 'G', 'J', 'K', 'L', 'M', 'N', 'O', 'Q', 'R', 'S', 'T', 'U', 'V', 'W']
```

### Opcodes Resolution

Some of the opcodes which are new or implemented for the first time in this bytecode are:

* Opcode "V": It reads the flag from a file, and the filename is read from `reg0`
* Opcode "(": Increments the value of PC by the value of `rega` (Equivalent to Jump instruction in x86)
* Opcode ")": Based on the outcome of the comparison, increases the PC by the value in `rega` by comparing the values of `regb` and `regc`. (Equivalent to JZ instruction in x86)
* Opcode "+": `regb` and `regc` undergo a less than operation, and the value of PC is increased in response to the comparison. (Equivalent to JLE instruction in x86)

### Building the GDB Script

Following some static and dynamic analysis, I used GDB to write a disassembler. Let's analyse the [disassembly](https://drive.google.com/file/d/1CPXO7833gePD_kqES89uriynDKYJSb9f/view?usp=drive_link) obtained from the first iteration of [gdb script](https://drive.google.com/file/d/1BdP_moTU6WybM5tFzP_pLKAmfyEIcaRY/view?usp=drive_link).

The beginning of the disassembly reveals that it constructs a filename to read the flag from, and the Opcode "V" reads this filename from `reg0`. Specifically, the generated filename is `flag.txt`.

Following this, the code calculates the length of our input flag and saves it in `reg1`.

![fsvm2](/images/fsvm2/fsvm2_1.png)


This part seems like the length check, because the length of the sample flag that I used in this instance is 48.
So now I know for the fact that the length of the flag should be 37.

![fsvm2](/images/fsvm2/fsvm2_2.png)

<br>

Now, let's re-run the disassembler with the correct flag length.

![fsvm2](/images/fsvm2/fsvm2_3.png)

We can clearly see the difference between the sequence of instructions executed if the flag length is correct

On further analysis, we can observe the following things:

- A number is generated, here `74`
- Last character of the input is extracted and stored in `reg1`
- Rest of the flag is stored in `reg8`

![fsvm2](/images/fsvm2/fsvm2_4.png)

- A loop runs for 74 times, and in each iteration it adds the value of the last character to `reg2`
- All that would need to be done is multiply the final character of the input by the number that is produced (74).

![fsvm2](/images/fsvm2/fsvm2_5.png)

Later the entire sum after 74 iterations is stored in `reg4`

The similar thing happens with all 37 characters of the flag.

At the end, the entire result of `reg4` is compared with a value.

![fsvm2](/images/fsvm2/fsvm2_6.png)


Hence, the First loop looks like this:

```!
flag[36]*74 + flag[35]*46 + flag[34]*70 + flag[33]*79 + flag[32]*58 + flag[31]*39 + flag[30]*41 + flag[29]*26 + flag[28]*46 + flag[27]*1 + flag[26]*79 + flag[25]*1 + flag[24]*27 + flag[23]*66 + flag[22]*34 + flag[21]*83 + flag[20]*97 + flag[19]*35 + flag[18]*94 + flag[17]*79 + flag[16]*37 + flag[15]*17 + flag[14]*76 + flag[13]*59 + flag[12]*97 + flag[11]*95 + flag[10]*1 + flag[9]*70 + flag[8]*26 + flag[7]*28 + flag[6]*99 + flag[5]*1 + flag[4]*95 + flag[3]*51 + flag[2]*10 + flag[1]*61 + flag[0]*44 == 172921
```



Since the 2 numbers are not same, the control skips to the end of the program.

![fsvm2](/images/fsvm2/fsvm2_7.png)

Although I was unsure of the number of such comparisons that would occur, I could still bypass that check in order to retrieve all of the accompanying hardcoded integer values and conditions.

The disassembly in that case would look like [this](https://drive.google.com/file/d/101U_o-6jrJ61Nx9Cr6Sp41whsAY1e0wj/view?usp=drive_link).

Additionally, since I am aware of the significance of each instruction, I may print only the cases that include the crucial constant values rather than the complete disassembly.



```python
import gdb
import os

f = []

def add_breakpoint(brk_p):
    gdb.execute(f"b *{brk_p}")

def read_reg(reg_name):
    x = open(reg_name)
    l = x.read()
    x.close()
    return l

def gdb_init():
    gdb.execute("set pagination off")
    gdb.execute("set disassembly-flavor intel")
    gdb.execute("file vm")
    add_breakpoint(0x000055555555676F)          #  opcode address
    add_breakpoint(0x0000555555557DAF)          #  S address
    add_breakpoint(0x0000555555556A2A)          #  *2 address
    add_breakpoint(0x0000555555556AD6)          #  + address
    add_breakpoint(0x00005555555571B2)          #  = address

    gdb.execute("run bytecode")

def file_init():
     with open('disas_he.txt', 'w') as fi:
        fi.write('Disassembly:\n')

def c():
    gdb.execute('continue')

def main():

    file_init()
    gdb_init()
    
    counter_s = 0

    while(1):
        x = gdb.execute('x/s $rbp-0x272',to_string=True)
        opcode = x[x.rfind('"')-1:-2]
        f.append(opcode)
        with open('disas_fin.txt', 'a') as fi:
            if str(opcode) == 'S':  
                c()
                reg_2 = gdb.execute('x/bx $rbp-0x272',to_string=True)
                reg_name2 = reg_2[reg_2.rfind('0x0')+3:]
                reg2 = read_reg('regs/reg'+reg_name2[0])
                if reg2 == 0:
                    reg2 = '0\n'
                fi.write(f"CASE S | reg{reg_name2[0]} = {reg2}\n")
            elif str(opcode) == '*':
                c()
                reg_a = read_reg('regs/rega')
                reg_b = read_reg('regs/regb')
                reg_c = read_reg('regs/regc')
                al = gdb.execute('print $al',to_string=True)
                if ' = 1' in al:
                    counter_s += 1
                    if counter_s > 38:
                        gdb.execute('set $al = 0')
                fi.write(f"CASE * | if {reg_c} != {reg_b} then PC += {reg_a}\n")
            elif str(opcode) == '+':
                c()
                reg_a = read_reg('regs/rega')
                reg_b = read_reg('regs/regb')
                reg_c = read_reg('regs/regc')
                fi.write(f"CASE + | if {reg_b.encode('utf-8')} < {reg_c.encode('utf-8')} then PC += {reg_a}\n")
            elif str(opcode) == '=':
                c()
                reg_2 = gdb.execute('x/bx $rbp-0x272',to_string=True)
                reg_name2 = reg_2[reg_2.rfind('0x0')+3:]
                reg2 = read_reg('regs/reg'+reg_name2[0])
                fi.write(f"CASE = | reg{reg_name2[0]} = {reg2}\n")
            elif str(opcode) == 'W':
                fi.write("CASE W | Exit\n")
            else:
                pass
        c() 
main()                 
```

The final disassembly would look like [this](https://drive.google.com/file/d/1x7_KKXELLYq2uVMlKIvQ1rAMwmRN-BCL/view?usp=drive_link).

This shows the final number that each loop checks against, as well as all the constants that are multiplied by each character. There are 37 such checks, which aligns with the length of the flag.


### Parsing the constant values

Let's disassemble that and get all those values.

I eliminated the initial portion of the disassembly that calculates the input flag's length in order to extract the constants.

Hence, we will perform the extraction on this modified [file](https://drive.google.com/file/d/11dxBl2rpXGscWrLyVBWN27fKplmzPoro/view?usp=drive_link).


```python!
# Extracting the constants

vals = []
eqt_vals = []

with open('disas_fin.txt', 'r') as f:
    x = f.readlines()
    for i in range(len(x)):
        if 'CASE S | reg8' in x[i]:
            jit = x[i+1]
            cons = jit[jit.rfind('< b')+4:jit.rfind('then')-2]
            vals.append(int(cons)+1)
        elif 'CASE * | if' in x[i]:
            jit = x[i]
            dd = jit[jit.rfind('!= ')+3:jit.rfind('then')-1]
            eqt_vals.append(int(dd))


print(vals)
print("\n")
print(eqt_vals)
print(" vals length:", len(vals))
print(" eqt_vals length: ", len(eqt_vals))
```

A constraint solver called z3 would be the best option to solve these contraints and obtain the flag.

So let's format these values in the z3 script

```python
vals = [74, 46, 70, 79, 58, 39, 41, 26, 46, 1, 79, 1, 27, 66, 34, 83, 97, 35, 94, 79, 37, 17, 76, 59, 97, 95, 1, 70, 26, 28, 99, 1, 95, 51, 10, 61, 44, 48, 74, 4, 92, 1, 26, 42, 94, 78, 44, 80, 62, 61, 27, 80, 48, 38, 46, 32, 92, 98, 66, 4, 7, 6, 61, 96, 71, 3, 32, 70, 94, 72, 60, 45, 9, 45, 64, 75, 63, 9, 40, 87, 26, 72, 30, 23, 49, 78, 66, 92, 23, 34, 42, 22, 7, 42, 12, 79, 5, 73, 27, 83, 88, 35, 1, 37, 28, 65, 30, 65, 67, 42, 1, 75, 85, 45, 73, 29, 53, 51, 90, 76, 37, 44, 7, 14, 47, 85, 57, 84, 5, 63, 96, 34, 44, 32, 47, 36, 25, 55, 28, 92, 47, 92, 48, 19, 7, 29, 2, 29, 52, 43, 57, 4, 11, 70, 95, 97, 49, 84, 80, 70, 58, 26, 7, 8, 67, 8, 87, 25, 62, 6, 90, 20, 92, 11, 99, 39, 61, 47, 55, 31, 67, 37, 46, 17, 91, 40, 70, 49, 17, 31, 29, 44, 32, 19, 6, 16, 7, 12, 58, 18, 1, 55, 22, 59, 12, 86, 74, 76, 15, 40, 65, 40, 36, 70, 23, 87, 32, 70, 32, 92, 79, 63, 99, 38, 38, 84, 80, 48, 100, 85, 71, 46, 73, 78, 96, 11, 49, 6, 32, 93, 74, 35, 6, 37, 83, 42, 86, 12, 10, 3, 26, 93, 57, 27, 55, 55, 76, 43, 52, 15, 41, 21, 83, 40, 1, 24, 69, 73, 55, 88, 94, 83, 15, 48, 22, 41, 58, 25, 35, 63, 63, 45, 39, 84, 38, 83, 84, 30, 80, 1, 94, 62, 56, 91, 9, 96, 63, 3, 3, 37, 31, 5, 90, 79, 10, 35, 35, 77, 13, 58, 8, 37, 29, 23, 61, 83, 9, 12, 95, 95, 57, 98, 11, 48, 87, 87, 75, 43, 51, 34, 44, 51, 6, 80, 75, 10, 63, 100, 44, 24, 79, 69, 98, 57, 48, 81, 53, 29, 43, 25, 14, 75, 11, 1, 26, 7, 24, 77, 52, 37, 64, 43, 45, 85, 3, 73, 73, 15, 55, 95, 76, 96, 42, 24, 91, 89, 59, 21, 65, 74, 79, 10, 77, 74, 58, 98, 84, 39, 36, 45, 86, 44, 39, 90, 81, 88, 33, 37, 52, 70, 42, 19, 39, 77, 53, 70, 8, 38, 69, 58, 34, 10, 47, 100, 13, 90, 21, 69, 36, 80, 15, 31, 36, 6, 95, 46, 76, 35, 30, 45, 38, 25, 9, 13, 52, 94, 99, 100, 54, 84, 18, 36, 23, 55, 34, 22, 37, 64, 50, 57, 76, 63, 99, 99, 33, 92, 35, 57, 83, 30, 86, 76, 79, 48, 54, 20, 28, 8, 36, 70, 83, 89, 21, 7, 63, 46, 5, 65, 42, 58, 17, 99, 97, 22, 45, 43, 90, 55, 5, 95, 32, 12, 82, 15, 2, 86, 77, 31, 79, 61, 1, 1, 32, 25, 37, 4, 17, 47, 81, 69, 94, 31, 59, 91, 60, 68, 30, 15, 100, 64, 62, 78, 89, 34, 51, 8, 72, 90, 35, 62, 17, 86, 2, 51, 38, 61, 85, 95, 55, 87, 21, 18, 19, 11, 61, 72, 38, 30, 33, 83, 9, 40, 60, 40, 14, 100, 76, 54, 19, 6, 91, 43, 13, 7, 38, 62, 76, 29, 64, 18, 51, 73, 14, 7, 97, 70, 24, 56, 68, 62, 57, 19, 46, 48, 18, 9, 22, 67, 12, 97, 20, 51, 71, 83, 51, 42, 21, 91, 15, 81, 26, 87, 25, 61, 48, 1, 50, 42, 46, 26, 88, 61, 18, 52, 79, 50, 86, 97, 4, 76, 27, 40, 63, 32, 79, 45, 83, 2, 7, 47, 32, 58, 57, 92, 47, 32, 7, 68, 78, 50, 52, 73, 98, 57, 29, 15, 22, 69, 27, 71, 57, 59, 77, 13, 95, 54, 97, 52, 82, 42, 86, 42, 91, 87, 84, 41, 57, 43, 30, 98, 46, 74, 22, 31, 44, 28, 16, 68, 70, 34, 88, 92, 36, 100, 98, 46, 38, 49, 70, 26, 1, 21, 20, 35, 60, 6, 71, 82, 58, 80, 25, 86, 74, 70, 98, 12, 33, 97, 36, 28, 56, 11, 58, 47, 38, 30, 52, 70, 93, 26, 83, 52, 5, 36, 44, 48, 11, 58, 91, 57, 62, 49, 47, 76, 14, 71, 78, 97, 14, 80, 43, 98, 5, 13, 40, 27, 92, 82, 10, 6, 29, 42, 7, 10, 42, 42, 40, 92, 53, 3, 55, 27, 62, 94, 39, 1, 21, 88, 39, 28, 7, 30, 98, 96, 16, 91, 99, 66, 46, 63, 16, 5, 68, 85, 62, 51, 25, 69, 12, 48, 30, 86, 43, 14, 99, 88, 94, 51, 24, 37, 21, 5, 83, 5, 15, 51, 11, 57, 57, 83, 69, 90, 78, 41, 72, 80, 37, 7, 99, 7, 7, 94, 12, 7, 14, 81, 6, 35, 85, 100, 36, 47, 53, 84, 63, 87, 6, 72, 85, 88, 33, 79, 2, 63, 12, 31, 18, 31, 33, 60, 50, 47, 45, 48, 28, 79, 9, 42, 68, 35, 14, 12, 42, 15, 67, 66, 47, 13, 25, 63, 64, 45, 28, 10, 98, 32, 14, 3, 55, 42, 19, 31, 68, 54, 45, 62, 40, 34, 88, 60, 33, 16, 37, 79, 14, 68, 28, 2, 35, 36, 10, 71, 45, 18, 2, 57, 72, 31, 73, 63, 43, 64, 30, 92, 35, 59, 54, 1, 53, 38, 67, 17, 55, 59, 98, 41, 71, 96, 30, 60, 4, 99, 7, 98, 50, 94, 24, 66, 40, 41, 89, 26, 65, 23, 36, 26, 31, 37, 96, 63, 89, 100, 51, 60, 3, 24, 69, 65, 98, 10, 13, 15, 7, 96, 54, 25, 97, 11, 69, 52, 86, 6, 69, 13, 17, 73, 33, 72, 2, 87, 55, 54, 69, 70, 8, 100, 7, 77, 48, 18, 15, 68, 96, 46, 38, 100, 89, 13, 21, 48, 41, 7, 4, 2, 55, 83, 16, 62, 96, 28, 30, 97, 18, 39, 8, 23, 81, 61, 60, 77, 23, 81, 45, 95, 11, 45, 96, 77, 21, 73, 39, 64, 66, 57, 40, 23, 62, 46, 74, 61, 77, 73, 82, 30, 53, 14, 66, 11, 79, 99, 75, 31, 58, 56, 39, 34, 75, 25, 6, 72, 71, 95, 45, 35, 79, 75, 59, 21, 55, 47, 58, 55, 78, 28, 45, 2, 5, 49, 37, 67, 13, 44, 29, 64, 97, 6, 34, 49, 63, 10, 32, 59, 12, 68, 29, 65, 52, 86, 40, 32, 57, 17, 90, 98, 96, 41, 92, 80, 3, 12, 41, 56, 88, 22, 54, 28, 26, 68, 47, 91, 48, 28, 3, 69, 49, 78, 42, 67, 81, 34, 49, 11, 71, 25, 19, 2, 23, 61, 29, 39, 77, 38, 99, 91, 16, 19, 3, 77, 58, 65, 44, 7, 33, 79, 45, 51, 28, 39, 17, 9, 24, 78, 10, 47, 73, 3, 70, 33, 70, 25, 93, 66, 24, 83, 29, 7, 4, 18, 28, 41, 45, 21, 56, 66, 14, 51, 4, 7, 26, 23, 48, 2, 65, 8, 33, 70, 47, 98, 7, 75, 85, 8, 24, 68, 25, 20, 76, 98, 27, 25, 32, 98, 94, 42, 95, 80, 77, 89, 17, 87, 89, 66, 3, 31, 95, 6, 99, 99, 98, 85, 70, 48, 40, 65, 37, 79, 44, 32, 55, 6, 93, 2, 39, 43, 60, 85, 1, 19, 50, 100, 61, 36, 80, 26, 44, 29, 9, 82, 72, 66, 36, 80, 36, 44, 24, 10, 17, 19, 69, 51, 86, 88, 95, 73, 96, 13, 85, 15, 87, 19, 100, 51, 96, 41, 60, 50, 42, 34, 32, 92, 29, 96, 77, 37, 49, 48, 5, 72, 84, 11, 33, 87, 22, 93, 100, 59, 26, 21, 29, 97, 10, 86, 10, 9, 3, 49, 59, 47, 25, 57, 34, 25, 95, 29, 6, 5, 80, 8, 98, 76, 90, 33, 34, 18, 26, 84, 76, 99, 1, 86, 87, 69, 21, 42, 65, 57, 71, 59, 15, 70, 15, 61, 73, 88, 76, 2, 61, 89, 47, 28, 47, 52, 33, 39, 13, 15, 5, 33, 28]
eqt_val = [172921, 165441, 154900, 154675, 164170, 140999, 181364, 176240, 153637, 168892, 199062, 158610, 170073, 157596, 168574, 152795, 162218, 198124, 175299, 179403, 145075, 168661, 161959, 134271, 155510, 182674, 169645, 162123, 188831, 151273, 149344, 145154, 167698, 172352, 187042, 155223, 164628]


print("from z3 import *\nlength = 37\n\nflag = [BitVec(f'flag_{i}', 8) for i in range(length)]\nsol = Solver()\nfor i in range(length):\n\tsol.add(flag[i] > 30)\n\tsol.add(flag[i] < 127)\n")

for k in range(37):
    print("sol.add(", end='')
    for i in range(37):
        print(f"flag[{37-i-1}]*{vals[k*37+i]} + ", end='')
    print(f"\b\b== {eqt_val[k]})\n")

print("if sol.check()==sat:\n\tm = sol.model()\n\tf = [chr(int(str(m[flag[i]]))) for i in range(len(m))]\n\tprint(''.join(f))\nelse:\n\tprint('unsat')")
```

#### The resultant z3 script:

```python
from z3 import *
length = 37

flag = [BitVec(f'flag_{i}', 8) for i in range(length)]
sol = Solver()

# Printable range contraint
for i in range(length):
	sol.add(flag[i] > 30)
	sol.add(flag[i] < 127)

sol.add(flag[36]*74 + flag[35]*46 + flag[34]*70 + flag[33]*79 + flag[32]*58 + flag[31]*39 + flag[30]*41 + flag[29]*26 + flag[28]*46 + flag[27]*1 + flag[26]*79 + flag[25]*1 + flag[24]*27 + flag[23]*66 + flag[22]*34 + flag[21]*83 + flag[20]*97 + flag[19]*35 + flag[18]*94 + flag[17]*79 + flag[16]*37 + flag[15]*17 + flag[14]*76 + flag[13]*59 + flag[12]*97 + flag[11]*95 + flag[10]*1 + flag[9]*70 + flag[8]*26 + flag[7]*28 + flag[6]*99 + flag[5]*1 + flag[4]*95 + flag[3]*51 + flag[2]*10 + flag[1]*61 + flag[0]*44 == 172921)

sol.add(flag[36]*48 + flag[35]*74 + flag[34]*4 + flag[33]*92 + flag[32]*1 + flag[31]*26 + flag[30]*42 + flag[29]*94 + flag[28]*78 + flag[27]*44 + flag[26]*80 + flag[25]*62 + flag[24]*61 + flag[23]*27 + flag[22]*80 + flag[21]*48 + flag[20]*38 + flag[19]*46 + flag[18]*32 + flag[17]*92 + flag[16]*98 + flag[15]*66 + flag[14]*4 + flag[13]*7 + flag[12]*6 + flag[11]*61 + flag[10]*96 + flag[9]*71 + flag[8]*3 + flag[7]*32 + flag[6]*70 + flag[5]*94 + flag[4]*72 + flag[3]*60 + flag[2]*45 + flag[1]*9 + flag[0]*45 == 165441)

sol.add(flag[36]*64 + flag[35]*75 + flag[34]*63 + flag[33]*9 + flag[32]*40 + flag[31]*87 + flag[30]*26 + flag[29]*72 + flag[28]*30 + flag[27]*23 + flag[26]*49 + flag[25]*78 + flag[24]*66 + flag[23]*92 + flag[22]*23 + flag[21]*34 + flag[20]*42 + flag[19]*22 + flag[18]*7 + flag[17]*42 + flag[16]*12 + flag[15]*79 + flag[14]*5 + flag[13]*73 + flag[12]*27 + flag[11]*83 + flag[10]*88 + flag[9]*35 + flag[8]*1 + flag[7]*37 + flag[6]*28 + flag[5]*65 + flag[4]*30 + flag[3]*65 + flag[2]*67 + flag[1]*42 + flag[0]*1 == 154900)

sol.add(flag[36]*75 + flag[35]*85 + flag[34]*45 + flag[33]*73 + flag[32]*29 + flag[31]*53 + flag[30]*51 + flag[29]*90 + flag[28]*76 + flag[27]*37 + flag[26]*44 + flag[25]*7 + flag[24]*14 + flag[23]*47 + flag[22]*85 + flag[21]*57 + flag[20]*84 + flag[19]*5 + flag[18]*63 + flag[17]*96 + flag[16]*34 + flag[15]*44 + flag[14]*32 + flag[13]*47 + flag[12]*36 + flag[11]*25 + flag[10]*55 + flag[9]*28 + flag[8]*92 + flag[7]*47 + flag[6]*92 + flag[5]*48 + flag[4]*19 + flag[3]*7 + flag[2]*29 + flag[1]*2 + flag[0]*29 == 154675)

sol.add(flag[36]*52 + flag[35]*43 + flag[34]*57 + flag[33]*4 + flag[32]*11 + flag[31]*70 + flag[30]*95 + flag[29]*97 + flag[28]*49 + flag[27]*84 + flag[26]*80 + flag[25]*70 + flag[24]*58 + flag[23]*26 + flag[22]*7 + flag[21]*8 + flag[20]*67 + flag[19]*8 + flag[18]*87 + flag[17]*25 + flag[16]*62 + flag[15]*6 + flag[14]*90 + flag[13]*20 + flag[12]*92 + flag[11]*11 + flag[10]*99 + flag[9]*39 + flag[8]*61 + flag[7]*47 + flag[6]*55 + flag[5]*31 + flag[4]*67 + flag[3]*37 + flag[2]*46 + flag[1]*17 + flag[0]*91 == 164170)

sol.add(flag[36]*40 + flag[35]*70 + flag[34]*49 + flag[33]*17 + flag[32]*31 + flag[31]*29 + flag[30]*44 + flag[29]*32 + flag[28]*19 + flag[27]*6 + flag[26]*16 + flag[25]*7 + flag[24]*12 + flag[23]*58 + flag[22]*18 + flag[21]*1 + flag[20]*55 + flag[19]*22 + flag[18]*59 + flag[17]*12 + flag[16]*86 + flag[15]*74 + flag[14]*76 + flag[13]*15 + flag[12]*40 + flag[11]*65 + flag[10]*40 + flag[9]*36 + flag[8]*70 + flag[7]*23 + flag[6]*87 + flag[5]*32 + flag[4]*70 + flag[3]*32 + flag[2]*92 + flag[1]*79 + flag[0]*63 == 140999)

sol.add(flag[36]*99 + flag[35]*38 + flag[34]*38 + flag[33]*84 + flag[32]*80 + flag[31]*48 + flag[30]*100 + flag[29]*85 + flag[28]*71 + flag[27]*46 + flag[26]*73 + flag[25]*78 + flag[24]*96 + flag[23]*11 + flag[22]*49 + flag[21]*6 + flag[20]*32 + flag[19]*93 + flag[18]*74 + flag[17]*35 + flag[16]*6 + flag[15]*37 + flag[14]*83 + flag[13]*42 + flag[12]*86 + flag[11]*12 + flag[10]*10 + flag[9]*3 + flag[8]*26 + flag[7]*93 + flag[6]*57 + flag[5]*27 + flag[4]*55 + flag[3]*55 + flag[2]*76 + flag[1]*43 + flag[0]*52 == 181364)

sol.add(flag[36]*15 + flag[35]*41 + flag[34]*21 + flag[33]*83 + flag[32]*40 + flag[31]*1 + flag[30]*24 + flag[29]*69 + flag[28]*73 + flag[27]*55 + flag[26]*88 + flag[25]*94 + flag[24]*83 + flag[23]*15 + flag[22]*48 + flag[21]*22 + flag[20]*41 + flag[19]*58 + flag[18]*25 + flag[17]*35 + flag[16]*63 + flag[15]*63 + flag[14]*45 + flag[13]*39 + flag[12]*84 + flag[11]*38 + flag[10]*83 + flag[9]*84 + flag[8]*30 + flag[7]*80 + flag[6]*1 + flag[5]*94 + flag[4]*62 + flag[3]*56 + flag[2]*91 + flag[1]*9 + flag[0]*96 == 176240)

sol.add(flag[36]*63 + flag[35]*3 + flag[34]*3 + flag[33]*37 + flag[32]*31 + flag[31]*5 + flag[30]*90 + flag[29]*79 + flag[28]*10 + flag[27]*35 + flag[26]*35 + flag[25]*77 + flag[24]*13 + flag[23]*58 + flag[22]*8 + flag[21]*37 + flag[20]*29 + flag[19]*23 + flag[18]*61 + flag[17]*83 + flag[16]*9 + flag[15]*12 + flag[14]*95 + flag[13]*95 + flag[12]*57 + flag[11]*98 + flag[10]*11 + flag[9]*48 + flag[8]*87 + flag[7]*87 + flag[6]*75 + flag[5]*43 + flag[4]*51 + flag[3]*34 + flag[2]*44 + flag[1]*51 + flag[0]*6 == 153637)

sol.add(flag[36]*80 + flag[35]*75 + flag[34]*10 + flag[33]*63 + flag[32]*100 + flag[31]*44 + flag[30]*24 + flag[29]*79 + flag[28]*69 + flag[27]*98 + flag[26]*57 + flag[25]*48 + flag[24]*81 + flag[23]*53 + flag[22]*29 + flag[21]*43 + flag[20]*25 + flag[19]*14 + flag[18]*75 + flag[17]*11 + flag[16]*1 + flag[15]*26 + flag[14]*7 + flag[13]*24 + flag[12]*77 + flag[11]*52 + flag[10]*37 + flag[9]*64 + flag[8]*43 + flag[7]*45 + flag[6]*85 + flag[5]*3 + flag[4]*73 + flag[3]*73 + flag[2]*15 + flag[1]*55 + flag[0]*95 == 168892)

sol.add(flag[36]*76 + flag[35]*96 + flag[34]*42 + flag[33]*24 + flag[32]*91 + flag[31]*89 + flag[30]*59 + flag[29]*21 + flag[28]*65 + flag[27]*74 + flag[26]*79 + flag[25]*10 + flag[24]*77 + flag[23]*74 + flag[22]*58 + flag[21]*98 + flag[20]*84 + flag[19]*39 + flag[18]*36 + flag[17]*45 + flag[16]*86 + flag[15]*44 + flag[14]*39 + flag[13]*90 + flag[12]*81 + flag[11]*88 + flag[10]*33 + flag[9]*37 + flag[8]*52 + flag[7]*70 + flag[6]*42 + flag[5]*19 + flag[4]*39 + flag[3]*77 + flag[2]*53 + flag[1]*70 + flag[0]*8 == 199062)

sol.add(flag[36]*38 + flag[35]*69 + flag[34]*58 + flag[33]*34 + flag[32]*10 + flag[31]*47 + flag[30]*100 + flag[29]*13 + flag[28]*90 + flag[27]*21 + flag[26]*69 + flag[25]*36 + flag[24]*80 + flag[23]*15 + flag[22]*31 + flag[21]*36 + flag[20]*6 + flag[19]*95 + flag[18]*46 + flag[17]*76 + flag[16]*35 + flag[15]*30 + flag[14]*45 + flag[13]*38 + flag[12]*25 + flag[11]*9 + flag[10]*13 + flag[9]*52 + flag[8]*94 + flag[7]*99 + flag[6]*100 + flag[5]*54 + flag[4]*84 + flag[3]*18 + flag[2]*36 + flag[1]*23 + flag[0]*55 == 158610)

sol.add(flag[36]*34 + flag[35]*22 + flag[34]*37 + flag[33]*64 + flag[32]*50 + flag[31]*57 + flag[30]*76 + flag[29]*63 + flag[28]*99 + flag[27]*99 + flag[26]*33 + flag[25]*92 + flag[24]*35 + flag[23]*57 + flag[22]*83 + flag[21]*30 + flag[20]*86 + flag[19]*76 + flag[18]*79 + flag[17]*48 + flag[16]*54 + flag[15]*20 + flag[14]*28 + flag[13]*8 + flag[12]*36 + flag[11]*70 + flag[10]*83 + flag[9]*89 + flag[8]*21 + flag[7]*7 + flag[6]*63 + flag[5]*46 + flag[4]*5 + flag[3]*65 + flag[2]*42 + flag[1]*58 + flag[0]*17 == 170073)

sol.add(flag[36]*99 + flag[35]*97 + flag[34]*22 + flag[33]*45 + flag[32]*43 + flag[31]*90 + flag[30]*55 + flag[29]*5 + flag[28]*95 + flag[27]*32 + flag[26]*12 + flag[25]*82 + flag[24]*15 + flag[23]*2 + flag[22]*86 + flag[21]*77 + flag[20]*31 + flag[19]*79 + flag[18]*61 + flag[17]*1 + flag[16]*1 + flag[15]*32 + flag[14]*25 + flag[13]*37 + flag[12]*4 + flag[11]*17 + flag[10]*47 + flag[9]*81 + flag[8]*69 + flag[7]*94 + flag[6]*31 + flag[5]*59 + flag[4]*91 + flag[3]*60 + flag[2]*68 + flag[1]*30 + flag[0]*15 == 157596)

sol.add(flag[36]*100 + flag[35]*64 + flag[34]*62 + flag[33]*78 + flag[32]*89 + flag[31]*34 + flag[30]*51 + flag[29]*8 + flag[28]*72 + flag[27]*90 + flag[26]*35 + flag[25]*62 + flag[24]*17 + flag[23]*86 + flag[22]*2 + flag[21]*51 + flag[20]*38 + flag[19]*61 + flag[18]*85 + flag[17]*95 + flag[16]*55 + flag[15]*87 + flag[14]*21 + flag[13]*18 + flag[12]*19 + flag[11]*11 + flag[10]*61 + flag[9]*72 + flag[8]*38 + flag[7]*30 + flag[6]*33 + flag[5]*83 + flag[4]*9 + flag[3]*40 + flag[2]*60 + flag[1]*40 + flag[0]*14 == 168574)

sol.add(flag[36]*100 + flag[35]*76 + flag[34]*54 + flag[33]*19 + flag[32]*6 + flag[31]*91 + flag[30]*43 + flag[29]*13 + flag[28]*7 + flag[27]*38 + flag[26]*62 + flag[25]*76 + flag[24]*29 + flag[23]*64 + flag[22]*18 + flag[21]*51 + flag[20]*73 + flag[19]*14 + flag[18]*7 + flag[17]*97 + flag[16]*70 + flag[15]*24 + flag[14]*56 + flag[13]*68 + flag[12]*62 + flag[11]*57 + flag[10]*19 + flag[9]*46 + flag[8]*48 + flag[7]*18 + flag[6]*9 + flag[5]*22 + flag[4]*67 + flag[3]*12 + flag[2]*97 + flag[1]*20 + flag[0]*51 == 152795)

sol.add(flag[36]*71 + flag[35]*83 + flag[34]*51 + flag[33]*42 + flag[32]*21 + flag[31]*91 + flag[30]*15 + flag[29]*81 + flag[28]*26 + flag[27]*87 + flag[26]*25 + flag[25]*61 + flag[24]*48 + flag[23]*1 + flag[22]*50 + flag[21]*42 + flag[20]*46 + flag[19]*26 + flag[18]*88 + flag[17]*61 + flag[16]*18 + flag[15]*52 + flag[14]*79 + flag[13]*50 + flag[12]*86 + flag[11]*97 + flag[10]*4 + flag[9]*76 + flag[8]*27 + flag[7]*40 + flag[6]*63 + flag[5]*32 + flag[4]*79 + flag[3]*45 + flag[2]*83 + flag[1]*2 + flag[0]*7 == 162218)

sol.add(flag[36]*47 + flag[35]*32 + flag[34]*58 + flag[33]*57 + flag[32]*92 + flag[31]*47 + flag[30]*32 + flag[29]*7 + flag[28]*68 + flag[27]*78 + flag[26]*50 + flag[25]*52 + flag[24]*73 + flag[23]*98 + flag[22]*57 + flag[21]*29 + flag[20]*15 + flag[19]*22 + flag[18]*69 + flag[17]*27 + flag[16]*71 + flag[15]*57 + flag[14]*59 + flag[13]*77 + flag[12]*13 + flag[11]*95 + flag[10]*54 + flag[9]*97 + flag[8]*52 + flag[7]*82 + flag[6]*42 + flag[5]*86 + flag[4]*42 + flag[3]*91 + flag[2]*87 + flag[1]*84 + flag[0]*41 == 198124)

sol.add(flag[36]*57 + flag[35]*43 + flag[34]*30 + flag[33]*98 + flag[32]*46 + flag[31]*74 + flag[30]*22 + flag[29]*31 + flag[28]*44 + flag[27]*28 + flag[26]*16 + flag[25]*68 + flag[24]*70 + flag[23]*34 + flag[22]*88 + flag[21]*92 + flag[20]*36 + flag[19]*100 + flag[18]*98 + flag[17]*46 + flag[16]*38 + flag[15]*49 + flag[14]*70 + flag[13]*26 + flag[12]*1 + flag[11]*21 + flag[10]*20 + flag[9]*35 + flag[8]*60 + flag[7]*6 + flag[6]*71 + flag[5]*82 + flag[4]*58 + flag[3]*80 + flag[2]*25 + flag[1]*86 + flag[0]*74 == 175299)

sol.add(flag[36]*70 + flag[35]*98 + flag[34]*12 + flag[33]*33 + flag[32]*97 + flag[31]*36 + flag[30]*28 + flag[29]*56 + flag[28]*11 + flag[27]*58 + flag[26]*47 + flag[25]*38 + flag[24]*30 + flag[23]*52 + flag[22]*70 + flag[21]*93 + flag[20]*26 + flag[19]*83 + flag[18]*52 + flag[17]*5 + flag[16]*36 + flag[15]*44 + flag[14]*48 + flag[13]*11 + flag[12]*58 + flag[11]*91 + flag[10]*57 + flag[9]*62 + flag[8]*49 + flag[7]*47 + flag[6]*76 + flag[5]*14 + flag[4]*71 + flag[3]*78 + flag[2]*97 + flag[1]*14 + flag[0]*80 == 179403)

sol.add(flag[36]*43 + flag[35]*98 + flag[34]*5 + flag[33]*13 + flag[32]*40 + flag[31]*27 + flag[30]*92 + flag[29]*82 + flag[28]*10 + flag[27]*6 + flag[26]*29 + flag[25]*42 + flag[24]*7 + flag[23]*10 + flag[22]*42 + flag[21]*42 + flag[20]*40 + flag[19]*92 + flag[18]*53 + flag[17]*3 + flag[16]*55 + flag[15]*27 + flag[14]*62 + flag[13]*94 + flag[12]*39 + flag[11]*1 + flag[10]*21 + flag[9]*88 + flag[8]*39 + flag[7]*28 + flag[6]*7 + flag[5]*30 + flag[4]*98 + flag[3]*96 + flag[2]*16 + flag[1]*91 + flag[0]*99 == 145075)

sol.add(flag[36]*66 + flag[35]*46 + flag[34]*63 + flag[33]*16 + flag[32]*5 + flag[31]*68 + flag[30]*85 + flag[29]*62 + flag[28]*51 + flag[27]*25 + flag[26]*69 + flag[25]*12 + flag[24]*48 + flag[23]*30 + flag[22]*86 + flag[21]*43 + flag[20]*14 + flag[19]*99 + flag[18]*88 + flag[17]*94 + flag[16]*51 + flag[15]*24 + flag[14]*37 + flag[13]*21 + flag[12]*5 + flag[11]*83 + flag[10]*5 + flag[9]*15 + flag[8]*51 + flag[7]*11 + flag[6]*57 + flag[5]*57 + flag[4]*83 + flag[3]*69 + flag[2]*90 + flag[1]*78 + flag[0]*41 == 168661)

sol.add(flag[36]*72 + flag[35]*80 + flag[34]*37 + flag[33]*7 + flag[32]*99 + flag[31]*7 + flag[30]*7 + flag[29]*94 + flag[28]*12 + flag[27]*7 + flag[26]*14 + flag[25]*81 + flag[24]*6 + flag[23]*35 + flag[22]*85 + flag[21]*100 + flag[20]*36 + flag[19]*47 + flag[18]*53 + flag[17]*84 + flag[16]*63 + flag[15]*87 + flag[14]*6 + flag[13]*72 + flag[12]*85 + flag[11]*88 + flag[10]*33 + flag[9]*79 + flag[8]*2 + flag[7]*63 + flag[6]*12 + flag[5]*31 + flag[4]*18 + flag[3]*31 + flag[2]*33 + flag[1]*60 + flag[0]*50 == 161959)

sol.add(flag[36]*47 + flag[35]*45 + flag[34]*48 + flag[33]*28 + flag[32]*79 + flag[31]*9 + flag[30]*42 + flag[29]*68 + flag[28]*35 + flag[27]*14 + flag[26]*12 + flag[25]*42 + flag[24]*15 + flag[23]*67 + flag[22]*66 + flag[21]*47 + flag[20]*13 + flag[19]*25 + flag[18]*63 + flag[17]*64 + flag[16]*45 + flag[15]*28 + flag[14]*10 + flag[13]*98 + flag[12]*32 + flag[11]*14 + flag[10]*3 + flag[9]*55 + flag[8]*42 + flag[7]*19 + flag[6]*31 + flag[5]*68 + flag[4]*54 + flag[3]*45 + flag[2]*62 + flag[1]*40 + flag[0]*34 == 134271)

sol.add(flag[36]*88 + flag[35]*60 + flag[34]*33 + flag[33]*16 + flag[32]*37 + flag[31]*79 + flag[30]*14 + flag[29]*68 + flag[28]*28 + flag[27]*2 + flag[26]*35 + flag[25]*36 + flag[24]*10 + flag[23]*71 + flag[22]*45 + flag[21]*18 + flag[20]*2 + flag[19]*57 + flag[18]*72 + flag[17]*31 + flag[16]*73 + flag[15]*63 + flag[14]*43 + flag[13]*64 + flag[12]*30 + flag[11]*92 + flag[10]*35 + flag[9]*59 + flag[8]*54 + flag[7]*1 + flag[6]*53 + flag[5]*38 + flag[4]*67 + flag[3]*17 + flag[2]*55 + flag[1]*59 + flag[0]*98 == 155510)

sol.add(flag[36]*41 + flag[35]*71 + flag[34]*96 + flag[33]*30 + flag[32]*60 + flag[31]*4 + flag[30]*99 + flag[29]*7 + flag[28]*98 + flag[27]*50 + flag[26]*94 + flag[25]*24 + flag[24]*66 + flag[23]*40 + flag[22]*41 + flag[21]*89 + flag[20]*26 + flag[19]*65 + flag[18]*23 + flag[17]*36 + flag[16]*26 + flag[15]*31 + flag[14]*37 + flag[13]*96 + flag[12]*63 + flag[11]*89 + flag[10]*100 + flag[9]*51 + flag[8]*60 + flag[7]*3 + flag[6]*24 + flag[5]*69 + flag[4]*65 + flag[3]*98 + flag[2]*10 + flag[1]*13 + flag[0]*15 == 182674)

sol.add(flag[36]*7 + flag[35]*96 + flag[34]*54 + flag[33]*25 + flag[32]*97 + flag[31]*11 + flag[30]*69 + flag[29]*52 + flag[28]*86 + flag[27]*6 + flag[26]*69 + flag[25]*13 + flag[24]*17 + flag[23]*73 + flag[22]*33 + flag[21]*72 + flag[20]*2 + flag[19]*87 + flag[18]*55 + flag[17]*54 + flag[16]*69 + flag[15]*70 + flag[14]*8 + flag[13]*100 + flag[12]*7 + flag[11]*77 + flag[10]*48 + flag[9]*18 + flag[8]*15 + flag[7]*68 + flag[6]*96 + flag[5]*46 + flag[4]*38 + flag[3]*100 + flag[2]*89 + flag[1]*13 + flag[0]*21 == 169645)

sol.add(flag[36]*48 + flag[35]*41 + flag[34]*7 + flag[33]*4 + flag[32]*2 + flag[31]*55 + flag[30]*83 + flag[29]*16 + flag[28]*62 + flag[27]*96 + flag[26]*28 + flag[25]*30 + flag[24]*97 + flag[23]*18 + flag[22]*39 + flag[21]*8 + flag[20]*23 + flag[19]*81 + flag[18]*61 + flag[17]*60 + flag[16]*77 + flag[15]*23 + flag[14]*81 + flag[13]*45 + flag[12]*95 + flag[11]*11 + flag[10]*45 + flag[9]*96 + flag[8]*77 + flag[7]*21 + flag[6]*73 + flag[5]*39 + flag[4]*64 + flag[3]*66 + flag[2]*57 + flag[1]*40 + flag[0]*23 == 162123)

sol.add(flag[36]*62 + flag[35]*46 + flag[34]*74 + flag[33]*61 + flag[32]*77 + flag[31]*73 + flag[30]*82 + flag[29]*30 + flag[28]*53 + flag[27]*14 + flag[26]*66 + flag[25]*11 + flag[24]*79 + flag[23]*99 + flag[22]*75 + flag[21]*31 + flag[20]*58 + flag[19]*56 + flag[18]*39 + flag[17]*34 + flag[16]*75 + flag[15]*25 + flag[14]*6 + flag[13]*72 + flag[12]*71 + flag[11]*95 + flag[10]*45 + flag[9]*35 + flag[8]*79 + flag[7]*75 + flag[6]*59 + flag[5]*21 + flag[4]*55 + flag[3]*47 + flag[2]*58 + flag[1]*55 + flag[0]*78 == 188831)

sol.add(flag[36]*28 + flag[35]*45 + flag[34]*2 + flag[33]*5 + flag[32]*49 + flag[31]*37 + flag[30]*67 + flag[29]*13 + flag[28]*44 + flag[27]*29 + flag[26]*64 + flag[25]*97 + flag[24]*6 + flag[23]*34 + flag[22]*49 + flag[21]*63 + flag[20]*10 + flag[19]*32 + flag[18]*59 + flag[17]*12 + flag[16]*68 + flag[15]*29 + flag[14]*65 + flag[13]*52 + flag[12]*86 + flag[11]*40 + flag[10]*32 + flag[9]*57 + flag[8]*17 + flag[7]*90 + flag[6]*98 + flag[5]*96 + flag[4]*41 + flag[3]*92 + flag[2]*80 + flag[1]*3 + flag[0]*12 == 151273)

sol.add(flag[36]*41 + flag[35]*56 + flag[34]*88 + flag[33]*22 + flag[32]*54 + flag[31]*28 + flag[30]*26 + flag[29]*68 + flag[28]*47 + flag[27]*91 + flag[26]*48 + flag[25]*28 + flag[24]*3 + flag[23]*69 + flag[22]*49 + flag[21]*78 + flag[20]*42 + flag[19]*67 + flag[18]*81 + flag[17]*34 + flag[16]*49 + flag[15]*11 + flag[14]*71 + flag[13]*25 + flag[12]*19 + flag[11]*2 + flag[10]*23 + flag[9]*61 + flag[8]*29 + flag[7]*39 + flag[6]*77 + flag[5]*38 + flag[4]*99 + flag[3]*91 + flag[2]*16 + flag[1]*19 + flag[0]*3 == 149344)

sol.add(flag[36]*77 + flag[35]*58 + flag[34]*65 + flag[33]*44 + flag[32]*7 + flag[31]*33 + flag[30]*79 + flag[29]*45 + flag[28]*51 + flag[27]*28 + flag[26]*39 + flag[25]*17 + flag[24]*9 + flag[23]*24 + flag[22]*78 + flag[21]*10 + flag[20]*47 + flag[19]*73 + flag[18]*3 + flag[17]*70 + flag[16]*33 + flag[15]*70 + flag[14]*25 + flag[13]*93 + flag[12]*66 + flag[11]*24 + flag[10]*83 + flag[9]*29 + flag[8]*7 + flag[7]*4 + flag[6]*18 + flag[5]*28 + flag[4]*41 + flag[3]*45 + flag[2]*21 + flag[1]*56 + flag[0]*66 == 145154)

sol.add(flag[36]*14 + flag[35]*51 + flag[34]*4 + flag[33]*7 + flag[32]*26 + flag[31]*23 + flag[30]*48 + flag[29]*2 + flag[28]*65 + flag[27]*8 + flag[26]*33 + flag[25]*70 + flag[24]*47 + flag[23]*98 + flag[22]*7 + flag[21]*75 + flag[20]*85 + flag[19]*8 + flag[18]*24 + flag[17]*68 + flag[16]*25 + flag[15]*20 + flag[14]*76 + flag[13]*98 + flag[12]*27 + flag[11]*25 + flag[10]*32 + flag[9]*98 + flag[8]*94 + flag[7]*42 + flag[6]*95 + flag[5]*80 + flag[4]*77 + flag[3]*89 + flag[2]*17 + flag[1]*87 + flag[0]*89 == 167698)

sol.add(flag[36]*66 + flag[35]*3 + flag[34]*31 + flag[33]*95 + flag[32]*6 + flag[31]*99 + flag[30]*99 + flag[29]*98 + flag[28]*85 + flag[27]*70 + flag[26]*48 + flag[25]*40 + flag[24]*65 + flag[23]*37 + flag[22]*79 + flag[21]*44 + flag[20]*32 + flag[19]*55 + flag[18]*6 + flag[17]*93 + flag[16]*2 + flag[15]*39 + flag[14]*43 + flag[13]*60 + flag[12]*85 + flag[11]*1 + flag[10]*19 + flag[9]*50 + flag[8]*100 + flag[7]*61 + flag[6]*36 + flag[5]*80 + flag[4]*26 + flag[3]*44 + flag[2]*29 + flag[1]*9 + flag[0]*82 == 172352)

sol.add(flag[36]*72 + flag[35]*66 + flag[34]*36 + flag[33]*80 + flag[32]*36 + flag[31]*44 + flag[30]*24 + flag[29]*10 + flag[28]*17 + flag[27]*19 + flag[26]*69 + flag[25]*51 + flag[24]*86 + flag[23]*88 + flag[22]*95 + flag[21]*73 + flag[20]*96 + flag[19]*13 + flag[18]*85 + flag[17]*15 + flag[16]*87 + flag[15]*19 + flag[14]*100 + flag[13]*51 + flag[12]*96 + flag[11]*41 + flag[10]*60 + flag[9]*50 + flag[8]*42 + flag[7]*34 + flag[6]*32 + flag[5]*92 + flag[4]*29 + flag[3]*96 + flag[2]*77 + flag[1]*37 + flag[0]*49 == 187042)

sol.add(flag[36]*48 + flag[35]*5 + flag[34]*72 + flag[33]*84 + flag[32]*11 + flag[31]*33 + flag[30]*87 + flag[29]*22 + flag[28]*93 + flag[27]*100 + flag[26]*59 + flag[25]*26 + flag[24]*21 + flag[23]*29 + flag[22]*97 + flag[21]*10 + flag[20]*86 + flag[19]*10 + flag[18]*9 + flag[17]*3 + flag[16]*49 + flag[15]*59 + flag[14]*47 + flag[13]*25 + flag[12]*57 + flag[11]*34 + flag[10]*25 + flag[9]*95 + flag[8]*29 + flag[7]*6 + flag[6]*5 + flag[5]*80 + flag[4]*8 + flag[3]*98 + flag[2]*76 + flag[1]*90 + flag[0]*33 == 155223)

sol.add(flag[36]*34 + flag[35]*18 + flag[34]*26 + flag[33]*84 + flag[32]*76 + flag[31]*99 + flag[30]*1 + flag[29]*86 + flag[28]*87 + flag[27]*69 + flag[26]*21 + flag[25]*42 + flag[24]*65 + flag[23]*57 + flag[22]*71 + flag[21]*59 + flag[20]*15 + flag[19]*70 + flag[18]*15 + flag[17]*61 + flag[16]*73 + flag[15]*88 + flag[14]*76 + flag[13]*2 + flag[12]*61 + flag[11]*89 + flag[10]*47 + flag[9]*28 + flag[8]*47 + flag[7]*52 + flag[6]*33 + flag[5]*39 + flag[4]*13 + flag[3]*15 + flag[2]*5 + flag[1]*33 + flag[0]*28 == 164628)


# Constraints for flag format "openECSC{...}"
sol.add(flag[0] == 111)
sol.add(flag[1] == 112)
sol.add(flag[2] == 101)
sol.add(flag[3] == 110)
sol.add(flag[4] == 69)
sol.add(flag[5] == 67)
sol.add(flag[6] == 83)
sol.add(flag[7] == 67)
sol.add(flag[8] == 123)
sol.add(flag[36] == 125)

if sol.check()==sat:
	m = sol.model()
	f = [chr(int(str(m[flag[i]]))) for i in range(len(m))]
	print(''.join(f))
else:
	print('unsat')

```

### Flag 

`openECSC{1ts_jus7_4n0ther_vm_582e5a5}`

### Closing Notes

I enjoyed how this VM was constructed and divided into two parts, each getting harder. This could be a good entry point into the world VM challenges.



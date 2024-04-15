+++
title = 'Fsvm : ecscCTF 2024'
date = 2024-04-01T18:59:02+05:30
featured_image = "/images/fsvm/fsvm_bg.png"
tags = ["Writeup"]
+++


I recently participated in ecscCTF 2024, mainly to try out the reversing challenges from the CTF. Subsequently, a two-part VM challenge caught my attention, called fsvm and fsvm2. 

These two use almost the same instruction set and different code dumps for emulation. The aim was to obtain the disassembly as soon as possible.

A huge shoutout to my teammate, [retr0ds](https://twitter.com/_retr0ds_), who collaborated with me to solve fsvm by helping me construct the emulator for the task.



### Triage

When we run the binary along with the provided bytecode, we encounter a prompt asking for a flag.  

![fsvm](/images/fsvm/fsvm_1.png)

To understand what's happening under the binary, I turned to IDA Pro for decompilation.


![fsvm](/images/fsvm/fsvm_2.png)

The `interpret` function seems like the program's heart, as it takes the `bytecode` file path as its argument. 

Before `interpret` is called, a directory named `regs` is created to store the values of the VM's registers and subsequenntly, this directory is deleted at the end of the program.

Within the `interpret` function lies a switch case structure, handling various opcodes and their corresponding operations. This is where the real magic happens

![fsvm](/images/fsvm/fsvm_3.png)

The bytecode file utilizes only a few specific opcodes, as listed below:

```python!
['*', '.', '0', '2', '3', '=', '>', '?', '@', 'A', 'B', 'C', 'E', 'Q', 'R', 'S', 'T', 'U', 'V', 'W']
```


#### Some important opcodes among these are - 

* Opcode "Q": Adds values from two registers and stores the result in a third
* Opcode "?": Concatenates values from two registers and stores the result in a third
* Opcode ">": Converts a register's value to its character form
* Opcode "T": Negates the number in a specified register
* Opcode "S": Pops out the last character from a register's value
* Opcode "=": Extracts the last byte of a register's value and stores it in another register 
* Opcode "*": Compares values in two registers and jumps to a specific instruction based on the comparison result

I explored two approaches to solve this challenge: building a complete VM emulator and utilizing a GDB script.

### Method 1

This approach involved creating a custom emulator to replicate the behavior of the VM used in the challenge.  
By examining the `interpret` function through decompilation, I discovered the presence of eight registers (reg0-reg7).


#### Emulator

```python!
def main():
    exit_flag = 0
    compared = []
    flag = []
    while True: 
        if exit_flag == 1:
            print("EXIT FLAG WAS SET. PROGRAM EXITED SUCCESSFULLY")
        print("Enter 1 to print registers and 2 to run emluator and 3 to restart :")
        choice = int(input())
        if choice == 1:
            print("Register 0: ", reg[0])
            print("Register 1: ", reg[1])
            print("Register 2: ", reg[2])
            print("Register 3: ", reg[3])
            print("Register 4: ", reg[4])
            print("Register 5: ", reg[5])
            print("Register 6: ", reg[6])
            print("Register 7: ", reg[7])
        elif choice == 2:
            fp = open('bytecode','rb')
            bytecode = fp.read()
            n = len(bytecode)
            i = 0

            reg = [""] * 8
            print(bytecode.count(b"="))
            while i < n:
                opcode = bytecode[i]

                match chr(opcode):
                    case "@":
                        reg[0] = "0"
                        print("CASE @ | reg[0] = 0")
                        i+=1
                    case "A":
                        print("CASE A | reg[1] = 0")
                        reg[1] = "0"
                        i+=1
                    case "B":
                        print("CASE B | reg[2] = 0")
                        reg[2] = "0"
                        i+=1

                    case "C":
                        print("CASE C | reg[3] = 0")
                        reg[3] = "0"
                        i+=1
                    case "D":
                        print("CASE D | reg[4] = 0")
                        reg[4] = "0"
                        i+=1
                    case "E":
                        print("CASE E | reg[5] = 0")
                        reg[5] = "0"
                        i+=1
                    case "F":
                        print("CASE F | reg[6] = 0")
                        reg[6] = "0"
                        i+=1
                    case "G":
                        print("CASE G | reg[7] = 0")
                        reg[7] = "0"
                        i+=1
                    case "R":
                        reg[bytecode[i+1]] = reg[bytecode[i+1]] + "1"
                        print(f"CASE R | reg[{bytecode[i+1]}] = reg[{bytecode[i+1]}] + 1 =========> {reg[bytecode[i+1]]}")
                        i+=2

                    case "Q":
                        A = reg[bytecode[i+1]]
                        B = reg[bytecode[i+2]]
                        result = int(A) + int(B)
                        reg[bytecode[i+3]] = str(result)
                        print(f"CASE Q | reg[{bytecode[i+3]}] = reg[{bytecode[i+1]}] + reg[{bytecode[i+2]}] ==========> reg[{bytecode[i+3]}] = {A} + {B} =======> {result} ")
                        i+=4
                    case "?":
                        A = reg[bytecode[i+1]]
                        B = reg[bytecode[i+2]]

                        compared.append(B)
                        result = str(A) + str(B)
                        reg[bytecode[i+3]] = result
                        print(f"CASE ? | reg[{bytecode[i+3]}] = reg[{bytecode[i+1]}] + reg[{bytecode[i+2]}] ==========> reg[{bytecode[i+3]}] = {A} + {B} =======> {result} ")
                        i+=4
                    case ">":
                        A = reg[bytecode[i+1]]
                        reg[bytecode[i+2]] = chr(int(A))
                        print(f"CASE > | reg[{bytecode[i+2]}] = chr(reg[{bytecode[i+1]}) ==========> reg[{bytecode[i+2]}] = chr({A}) =======> {chr(int(A))} ")
                        i+=3

                    case "T":
                        A = reg[bytecode[i+1]]
                        if "-" not in A:
                            reg[bytecode[i+1]] = str("-" + str(ord(A)))
                            print(f"CASE T | reg[{bytecode[i+1]}] = -reg[{bytecode[i+1]}] ==========> reg[{bytecode[i+1]}] = - + {A}")
                            i+=2
                        else:
                            print(f"CASE T | reg[{bytecode[i+1]}] = -reg[{bytecode[i+1]}] ==========> reg[{bytecode[i+1]}] = {A}")
                            i += 2

                    case "U":
                        A = reg[4]
                        print(f"CASE U | print reg[4] =============> ", A)
                        i+=1
                    
                    case ",":
                        reg[0] = 0
                        print("CASE , | reg[0] = 0")
                        i+=1
                    case "-":
                        reg[1] = 0
                        print("CASE - | reg[1] = 0")
                        i+=1
                    case ".":
                        reg[2] = 0
                        print("CASE . | reg[2] = 0")
                        i+=1
                    case "/":
                        reg[3] = 0
                        print("CASE / | reg[3] = 0")
                        i+=1
                    case "0":
                        reg[4] = 0
                        print("CASE 0 | reg[4] = 0")
                        i+=1
                    case "1":
                        reg[5] = 0
                        print("CASE 1 | reg[5] = 0")
                        i+=1
                    case "2":
                        reg[6] = 0
                        print("CASE 2 | reg[6] = 0")
                        i+=1
                    case "3":
                        reg[7] = 0
                        print("CASE 3 | reg[7] = 0")
                        i+=1
                    
                    case "V":
                        reg[0] = input("Case V | GET INPUT reg[0] =============> ")
                        i+=1
                    case "W":
                        exit_flag = 1
                    case "=":
                        A = reg[bytecode[i+1]]
                        reg[bytecode[i+2]] = A[-1]
                        print(f"CASE = | reg[{bytecode[i+2]}] = reg[{bytecode[i+1]}][-1] ==========> reg[{bytecode[i+2]}] = {A}[-1] =======> {A[-1]} ")
                        flag.append(reg[1])
                        i += 3  
                    case "S":
                        A = reg[bytecode[i+1]]
                        size_A = len(A)
                        if size_A > 0:
                            A = list(A)
                            A.pop()
                            A = "".join(A)
                        reg[bytecode[i+1]] = A
                        print(f"CASE S | reg[{bytecode[i+1]}] = reg[{bytecode[i+1]}][:-1] ==========> reg[{bytecode[i+1]}] = {A}[:-1] =======> {A} ")
                        i += 2
                    case "*":
                        A = reg[5]
                        B = reg[6]
                        C = reg[7]
                        if B != C:
                            i = int(A)
                            print(f"CASE * | i = reg[5] =============> ", i)
                        else:
                            i+=1
                            print(f"CASE * | i = i+1 =============> ", i)

                    case _:
                        print("DEFAULT CASE")
                        print(flag)
                        break
        elif choice == 3:
            exit(0)             


if __name__ == '__main__':
    main()
```

Analysis of the disassembled code revealed a byte-by-byte comparison taking place. 
Basically, the program extracts the last character from your input and compares it against a dynamically generated value stored in reg1 each time the "=" opcode is encountered.


To obtain the values that are being compared, we can simply store the value of `reg1` read at opcode "=". 


Output:

```
DEFAULT CASE
['125', '-430', '52', '99', '55', '56', '0101', '52', '99', '109', '0118', '0121', '0115', '97', '0101', '0114', '0101', '0112', '0117', '0115', '123', '67', '83', '67', '69', '0110', '0101', '0112', '-419']
```

The remaining characters make the flag, with the exception of the two that spit out an unprintable character. At this point, all I did to get the flag was estimate those two characters.

Flag: `openECSC{supereasyvmc4e87c4d}`


### Method 2

Since we know that on reading the value of `reg1` during the execution of opcode "=" gives out the flag characters that are being checked, we can write a gdb script to set a breakpoint at that point and read the values straight from the `reg1` file under `regs` directory created by the binary itself.


This method we utilise the debugging capabilities of GDB to narrow down the computation required to obtain the solution. Our understanding from the previous parts was that the "=" opcode extracts the last character from the flag input and compares it against a value stored in `reg1` which is the correct flag byte. 

By crafting a GDB script, we could set a breakpoint at the "=" opcode.  Upon reaching this breakpoint, the script would directly read the value stored in `reg1` within the `regs` directory created by the program itself.  

```python!
import gdb
import os

flag = ''

gdb.execute("set pagination off")
gdb.execute("set disassembly-flavor intel")
gdb.execute("file vm")

address = 0x0000555555556ECA
gdb.execute(f"b *{address}")

gdb.execute("run bytecode < flag.txt")

for i in range(29):
    x = open(r'regs/reg1')
    l = x.read()
    flag += (chr(int(str(l))))
    x.close()
    gdb.execute('c')

print(flag[::-1])
print("Done")
```

Output:

`openECSC{supereasyvmc4e87c4d}`

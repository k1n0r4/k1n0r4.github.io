+++
title = 'Chronos VM'
date = 2024-03-03T16:27:51+05:30
featured_image = "/images/chronos_vm/chronos_bg_n.jpg"
tags = ["VM"]
+++

This virtual machine (VM) is custom-designed for a specific purpose. It includes standard instructions for data movement and arithmetic operations, along with unique jump instructions that extend the capabilities of typical jumping behavior. The VM also has a limited set of flags that can be expanded based on specific needs. 

It operates using 32-bit opcodes and registers. 
This VM is created as a part of a challenge for an International CTF, bi0s CTF 2024.

In that challenge, the VM performs bit manipulation on the user input which demands reversing the logic of the bit operations and retrieve back the original bytes.



## Registers

There are in total 8 General Purpose registers and 4 Special registers

### General Registers

* 700: S0
* 701: S1
* 702: S2
* 703: S3
* 704: S4
* 705: S5
* 706: S6
* 707: S7


### Special Registers

* 708: MP - Memory pointer
* PC - Program counter
* SP - Stack pointer
* BP - Base pointer

#### Flags
* EF - Exit Flag
* ZF - Zero Flag

## Instruction Set


| Opcodes  | Mnemonic | Description | Use Case of  Opcodes  |
| ---- | ---- | ---- | ---- |
| 1200 (10) | MOVR | "Moves" a value from one register to another (It does not copy the value but actually moves it) | MOVR S0, S1<br>S1 --> S0 |
| 1201 (8) | COPI | Typical 'mov' operation from x86 with an immediate value | COPI S0, 4<br>4 --> S0 |
| 1202 (10) | COPR | Typical mov operation from x86 | COPR S0, S1<br> |
| 1203 (8) | ADDI | Add the values of the register and the immediate value and store it in first operand | ADDI S0, 4<br>S0 +=  4 |
| 1204 (10) | ADDR | Add the values of the 2 register and save the result in the first register | ADDR S0, S1<br>S0 = S0 + S1 |
| 1205 (8) | SUBI | Subtract the values of the register and the immediate value and store it in first operand | SUBI S0, 4<br>S0 -=  4 |
| 1206 (10) | SUBR | Subtract the values of the 2 register and save the result in the first register | SUBR S0, S1<br>S0 = S0 - S1 |
| 1207 (8) | XORI | Xor the values of the register and the immediate value and store it in first operand | XORI S0, 4<br>S0 ^=  4 |
| 1208 (10) | XORR | Xor the values of the 2 register and save the result in the first register | XORR S0, S1<br>S0 = S0 ^ S1 |
| 1209 (8) | DIVI | Divide the value of the register by the immediate value and store it in first operand | DIVI S0, 4<br>S0 /=  4 |
| 1210 (10) | DIVR | Divide the value of the 1st register by 2nd register and save the result in the first register | DIVR S0, S1<br>S0 = S0 / S1 |
| 1211 (8) | MULI | Multiply the values of the register and the immediate value and store it in first operand | MULI S0, 4<br>S0 *=  4 |
| 1212 (10) | MULR | Multiply the values of the 2 register and save the result in the first register | MULR S0, S1<br>S0 = S0 * S1 |
| 1213 (7) | POPA | Pops the top value of the stack and stores it in the given register. SP --> SP + 1 | POPA S0<br>[SP] --> S0 |
| 1214 (7) | PUSH | The value in the register is pushed on the top of the stack. SP --> SP - 1 | PUSH S0<br>S0 --> [SP-1] |
| 1215 (8) | LOAD | Load the data from the memory to register. The offset of the MP is given as the 2nd argument | LOAD S0, 1<br>[MP+1] -->S0 |
| 1216 (7) | STOR | Store the data from register to memory. <br>MP --> MP + 1 | STOR S0<br>S0 --> [MP] |
| 1217 (~) | CALL | Calls the function given, the length of the function name and then the function name is provided | CALL 5 func0 |
| 1218 (7) | INCR | Increments the value of the register by 1 and store it in the same register | INCR S0<br>S0 += 1 |
| 1219 (7) | DECR | Decrements the value of the register by 1 and store it in the same register | DECR S0<br>S0  -= 1 |
| 1220 (8) | LD_D | Another version of Load, where the value from the memory is deleted when it's loaded into the register | LD_D S1, 1<br>[MP+1] -->S1 |
| 1221 (8) | SHRI | Shift Right the value of the register by the given value | SHRI S0, 1<br>S0 = S0 >>1 |
| 1222 (10) | SHRR | Shift Right the value of the register by the given value within a register | SHRR S0, S1<br>S0 = S0 >>S1 |
| 1223 (8) | SHLI | Shift Left the value of the register by the given value | SHLI S0, 1<br>S0 = S0 << 1 |
| 1224 (10) | SHLR | Shift Left the value of the register by the given value within a register | SHLR S0, S1<br>S0 = S0 << S1 |
| 1225 (~) | PRIM | Print the message, the length of the message and the message is given | PRIM 5 Hello |
| 1226 (7) | PRIR | Prints the value of the register | PRIR S0 |
| 1227 (8) | COPS | Copies the value from the stack to the register given | COPS S0,4<br>[SP+4] --> S0 |
| 1228 (~) | JZD | Increment the PC by the value given if the ZF is set to 1 | JZD 4<br>PC += 4 |
| 1229 (~) | JNZD | Increment the PC by the value given if the ZF is set to 0 | JNZD 4<br>PC += 4 |
| 1230 (~) | JMPD | Increment the PC by the value given | JMPD 4<br>PC += 4 |
| 1231 (~) | SCAN | Takes input in 3 forms, integer, character and string<br>The type of data needs to be specified as 'i' for integer, 'c' for character and 's' for string<br>Integer input is stored in the register specified<br>Character input is stored directly in the memory<br>String input is converted to it's base64 form and then stored in the memory | SCAN i S0<br>input --> S0 |
| 1232 (8) | CMPI | Compares the value in the register with the immediate value and sets the zero flag accordingly | CMPI S0, 3 |
| 1233 (10) | CMPR | Compares the values in the 2 registers and sets the zero flag accordingly | CMPI S0, S1 |
| 1234 (4) | EXIT | Sets the Exit flag as 1 | EXIT |
| 1235 (8) | ANDI | Bitwise And operation is performed on the value of the register and the immediate value and is stored it in the first operand | ANDI S0, 4<br>S0 &=  4<br> |
| 1236 (10) | ANDR | Bitwise And operation is performed on the values of the 2 registers and result is stored in the first operand | ANDR S0, S1<br>S0 = S0 & S1 |
| 1237 (8) | ORRI | Bitwise Or operation is performed on the value of the register and the immediate value and is stored it in the first operand | ANDI S0, 4<br>S0 \|=  4 |
| 1238 (10) | ORRR | Bitwise Or operation is performed on the values of the 2 registers and result is stored in the first operand | ORRR S0, S1<br>S0 = S0 \| S1 |
| 1239 (~) | JZU | Decrement the PC by the value given if the ZF is set to 1 | JZU 4<br>PC -= 4 |
| 1240 (~) | JNZU | Decrement the PC by the value given if the ZF is set to 0 | JNZU 4<br>PC -= 4 |
| 1241 (~) | JMPU | Decrement the PC by the value given | JMPD 4<br>PC -= 4 |
| 1242 (~) | JNZ | Place the PC at the offset mentioned if the zero flag is set to 0 | JNZ 4<br>PC = 4 |
| 1243 (~) | JZ | Place the PC at the offset mentioned if the zero flag is set to 1 | JZ 4<br>PC = 4 |


## Application

This VM is created as a part of a challenge for an International CTF, bi0s CTF 2024
In that challenge, the VM performs bit manipulation on the user input which demands reversing the logic of the bit operations and retrieve back the original bytes.
### Control Flow

1. Prompt for input
2. Take input and format it
3. Store the modified input in memory
4. Store the same in the stack
5. Convert into binary and store it in stack and remove the input from memory
6. Classified array is computed and stored in memory

### Disassembly

```
;S0 has the modified input length

		COPI 	MP, 0
		PRIM 16 Enter the flag: 
		SCAN s

		COPI 	S1, 0
		COPI	MP, 0
48: 	LOAD 	S2, [MP]
		INCR 	MP
		INCR	S1
		PUSH 	S2
		CMPR  	S1, S0
87: 	JNZU  	39        ;48


92: 	COPI 	S7, 0
100:	DECR	MP
107:	LD_D	S1, [MP]
115:	COPI	S2, 0
123:	COPR	S3, S1
133:	ANDI	S3, 1
141:	PUSH	S3
148:	SHRI	S1, 1
156:	INCR	S2
163:	CMPI	S2, 8
171:	JNZU	48 			;123
176:	INCR	S7
183:	CMPR	S7, S0
193:	JNZU	93  		;100


198:	COPR 	S1, S0
208:	DECR 	S1
215:	COPI 	S2, 0
223:	COPS	S3, [SP]
231:	COPS	S4, [SP+8]
239:	XORR	S3, S4
249:	STOR 	S3
256:	COPS	S3, [SP+1]
264:	COPS	S4, [SP+9]
272:	XORR	S3, S4
282:	STOR 	S3
289:	COPS	S3, [SP+2]
297:	COPS	S4, [SP+10]
305:	XORR	S3, S4
315:	STOR 	S3
322:	COPS	S3, [SP+1]
330:	COPS	S4, [SP+8]
338:	XORR	S3, S4
348:	STOR 	S3
355:	COPS	S3, [SP]
363:	COPS	S4, [SP+9]
371:	XORR	S3, S4
381:	STOR 	S3
388:	COPS	S3, [SP+2]
396:	COPS	S4, [SP+9]
404:	XORR	S3, S4
414:	STOR 	S3
421:	COPS	S3, [SP+1]
429:	COPS	S4, [SP+10]
437:	XORR	S3, S4
447:	STOR 	S3
454:	COPS	S3, [SP]
462:	COPS	S4, [SP+2]
470:	COPS 	S5, [SP+9]
478:	XORR	S3, S4
488:	XORR	S3, S5
498:	STOR 	S3
505:	POP		S5
512:	INCR	S2
519:	CMPI	S2, 6
527:	JNZ 	223			
532:	POP 	S5
539:	POP		S5
546:	DECR	S1
553:	CMPI 	S1, 0
561:	JNZ 	215			
566:	CMPI	MP, 144
574:	JNZD 	15        ;589
579:	PRIM 5  Wrong
589:	EXIT  
```


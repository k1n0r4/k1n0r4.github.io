+++
title = 'Exploring Katanaverse Implementation'
date = 2024-02-02T12:45:19+05:30
featured_image = "/images/katana/katana_bg.jpg"
tags = ["katanaverse", "quantum", "VM"]
+++

In this detailed explanation, I will walk you through the process of constructing Katanaverse, the binary utilized as a bi0sCTF 2024 Challenge. This challenge falls within the domain of reverse engineering and requires participants to employ puzzle-solving skills. Katanaverse presents a unique blend of custom architecture and concepts from Quantum Computing.


In Katanaverse 0.0, participants tackled the Weighted Maxcut Problem, aiming to split connected dots on a graph into two groups to maximize the weight of lines between them. This demanded strategic thinking and experimentation. In Katanaverse 1.0, participants employed a straightforward but exhaustive approach, trying every possible solution until finding the best one.

Also you can find the source codes and binaries of both the challenges on my [Github repo](https://github.com/k1n0r4/Katanaverse).

## Overview of the Challenge

The entire challenge is divided into multiple parts
* VM which includes the bit manipulation and the creation of classified array
* Generation of constants
    1. AES key generation via the known part of classified 2D array
    2. srand seed generation using the input trigger and the known part of the classfied 2D array
* Constraint check for classified array using rand values
* Generate 12 points for each group of classified array 
* Convert alpha beta notation of points into xyz coordinates
* Apply Quantum Approximate Optimization Algorithm on those 12 points
* The result of the algorithm is formatted and aes encrypted
* The encrpted value is then compared against the encrypted bytes of the correct flag

<br/>

## #1 VM

### Control flow of the VM

1. Prompt for Flag
2. Takes the input
3. Transforms the flag into its string encoded in base 64.
4. Register S0 stores the updated flag's length.
6. Binary representations of flag bytes are created and placed on the stack.
7. As a result, each time a flag byte is converted, the byte is removed from memory.
8. Memory is discovered to be empty following the conversion.
9. A bit modification that is kept in memory is carried out using stored binary from the stack.
10. VM exits
11. It replicates its memory into a "classified" 2D array 
12. It stores the altered input length from register S0 in a new variable for use in future applications.

The classified array is a 2D array of size `(inp_len-1)*6 X 8`
8 corresponds to each bit of the byte

For more details on the Chronos VM, you can refer to my blog post on [Chronos VM](https://k1n0r4.github.io/post/chronos_vm/).


### Bit manipulation

![katanaverse](/images/katana/katana_1.jpeg)

## #2 Constant Generation

Since the flag format is included in the creation of classified array, we know the initial few bytes of classified array

Those known bytes are used as aes_key

AES Key Generation
```clike=
uint8_t aes_key[16];

    for(int i =0;i<16;i++)
    {
        aes_key[i] = classified[i][7] * 1 + classified[i][6] * 2 + classified[i][5] * 4 + classified[i][4] * 8 + classified[i][3] * 16 + classified[i][2] * 32 + classified[i][1] * 64 + classified[i][0] * 128 ;
    }
```

Similarly, seed for srand is also determined using these known classified bytes

```clike=
int seed_gen = 0;
    for(int i =0;i<8;i+=2)
    {
        seed_gen+=classified[i][3]*1 +classified[i][2]*2 + classified[i][1]*4 + classified[i][0]*8;
        seed_gen+=classified[i+1][7]*1 +classified[i+1][6]*2 + classified[i+1][5]*4 + classified[i+1][4]*8;
    }


    char g[1];
    printf("\nEnter the trigger: ");
    scanf("%c", &g[0]);

    if(g[0]=='r')
    {
        srand(seed_gen^g[0]);
    }
    else{
        printf("Wrong trigger\n");
        exit(0);
    }
```

An input trigger is also used in the seed generation

<br>

## #3 Constraints


`rand()` function is used to generate 5 unique numbers within the range of 0 to 5

Hence for every 6 bytes, a constraint between the first and last bit for that byte is given
For example, if the numbers generated are 0,1,2,3,4 then first 5 bytes of that group of 6 is taken and constraint between the first and last bit is given as 0 or 1
0 means the bits are not same, they are either 0 and 1 or 1 and 0
1 means the bits are same, they are either 0 and 0 or 1 and 1

These constraints are provided to make bruting less tedious

`conditions` array has those constraints

In katanaverse 0.0, the modified input length is 25
So the classified array (inp_len-1) X 6 is 144

Each 6 bytes correspond to the pair of bytes of our modified input

hence if we take first 6 bytes of classified array and given the constraints for 5 bytes given, there are only 8 possibilities

```clike
int conditions[120] = {1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1};
    int p=0;
    for(int j =0;j<(inp_len-1)*6;j+=6)
    { 
        int rands[5];
        rands[0] = rand()%4;
        for(int i=1; i<3; i++)
        {
            int r = rand()%4;
            while(r == rands[i-1] || r == rands[0])
            {
                r = rand()%4;
            }
            rands[i] = r;
        }
        rands[3] = 4;
        rands[4] = 5;

        for(int k=0;k<5;k++)
        {
            int g = (classified[j+rands[k]][0] == classified[j+rands[k]][7]);
            if(g != conditions[p])
            {
                printf("Wrong Input\n");
                exit(0);
            }
            p+=1;
        }
    }

```

<br>

## #4  Generation of 12 points

Each time 6 bytes of classified array is taken and 6 X 8 bits are divided into 12 groups of 4 bits(nibble) each

These 4 bits (nibble) at a time determine which operation will be performed
There are 16 operations named as Blaze, Horizon, Nova, Quantum, Echo,...


![katanaverse](/images/katana/katana_2.jpeg)


These are the combination of coordinate shift that will be performed on the given qubit along it's x,y and z axis
This will change the position of the qubit

These operations can be visualised using the [Bloch Sphere](https://bits-and-electrons.github.io/bloch-sphere-simulator/#{%22blochSphereStateProperties%22:{%22theta%22:%22+0.0000%22,%22phi%22:%22+90.0000%22},%22customGatesProperties%22:{},%22lambdaGatesProperties%22:{%22polarAngle%22:%220%22,%22azimuthAngle%22:%220%22}})

Initial qubit position is given by two variables alpha and beta as 1 + 0i , 0 + 0i

When an operation is performed on the qubit, it will undergo change in the alpha and beta values of the qubit

Hence, taking inital qubit state as given above we perform 12 such operations on the qubit and store the state of the qubit after every operation
Hence we have 12 combinations of alpha and beta at the end for each 6 bytes of the classified array
These alpha and beta are converted to their respective x,y,z coordinate form

The function `choice` is used to decide which operation will be performed based on the 4 bits

```clike!
for(int i=0;i<(inp_len-1)*6;i+=6)
    {
        double complex alpha = 1 + 0*I;
        double complex beta = 0 + 0*I;
        double complex alts[12][2];

        for(int j=0;j<6;j++)
        {
            int com[2] = {0,0};
            com[0] = classified[i+j][0]*8 + classified[i+j][1]*4 + classified[i+j][2]*2 + classified[i+j][3]*1;
            com[1] = classified[i+j][4]*1 + classified[i+j][5]*2 + classified[i+j][6]*4 + classified[i+j][7]*8;
            choice(com[0], alpha, beta, &alts[j*2][0], &alts[j*2][1], subtract_key, xor_key); 
            choice(com[1], alts[j*2][0], alts[j*2][1], &alts[j*2+1][0], &alts[j*2+1][1], subtract_key, xor_key); 
            alpha = alts[j*2+1][0];
            beta = alts[j*2+1][1];
        }

        for(int k=0;k<12;k++)
        {
            alpha_beta_to_xyz(alts[k][0], alts[k][1], coordinates[i*2+k]);
        }
    }
```

The functions Blaze, Horizon, Nova, Quantum, Echo ... represent the operations and technically to perform these operation is called to "pass the qubit through the custom gates"

Within each function, there is an encrypted shellcode, which on decrypting is the qiskit code to implement that specific gate (operation/coordinate shift in 3D cuz it is a sphere)

It will look something like this

```
OPENQASM 2.0;
include "qelib1.inc";
gate disentangler_dg q0 {  }
gate state_preparation(param0,param1) q0 { disentangler_dg q0; }
gate initialize(param0,param1) q0 { reset q0; state_preparation(alpha,beta) q0; }
qreg q2[1];
creg c2[1];
initialize(alpha,beta) q2[0];
rx(pi/4) q2[0];
ry(pi/3) q2[0];
rz(pi/4) q2[0];
measure q2[0] -> c2[0]; 
```

Here the operation performed is, 
1. 45 degree along x axis
2. 60 degree along y axis
3. 45 degree along z axis

Likewise each function has different operations
This is where the given binary will cease to execute normally and throw a segmantation fault because you need a proper platform to execute this qiskit code

It can be done in [Quantum Lab provided by IBM](https://quantum.ibm.com/lab)

Although, conversion from alpha beta to xyz coordinates and vice versa can be done here

Hence at the end of this step we have 12 points for each group of 6 bytes of the classified array

We have 24 sets of 12 points

```
Note: For the encrypting the shellcode, the constants subtract_key and xor_key are determined using the known part of the classified array, the routine is a simple subtract and xor of each byte in the order
```

<br>

## #5 QAOA

Quantum Approximate Optimization Algorithm is a way to divide the given set of points in a way that they are equally divided

QAOA is used to solve [max cut problem](https://qiskit-community.github.io/qiskit-optimization/tutorials/06_examples_max_cut_and_tsp.html) here

The weights are the distances between the points

We make 41 such pairs and calculate distances between these pairs and assign them as their weights

So now we have a network of 12 points and 41 connections connecting them

Now QAOA is applied on this network and based on their weights each point will be assigned to one of the group, that is Group A or Group B
Group A say is represented by 0, so Group B is represented by 1

so the output of the QAOA applied on one network will be 12 bits

For example: 001111001010

Hence we can say that 1ast and second point is assigned to Group A, then next 4 points are assigned to Group B and it goes on

Thus, all the points are divided in 2 groups

In the binary, the function that is supposed to perform qaoa is broken, there are only encrypted bytes which give out a hint about using qaoa

This is the decrypted string part under qaoa for katanaverse 0.0

```
Oh no! Not only is this function flawed, it is nonexistent. Could you please solve this?

**Static noise

Agent, We happened to record a conversation between Martin and his subordinates. For your reference, here is the transcript of our conversation.

Subordinate 1: Is it possible for an ordinary person to open the vault?
Martin: No, definitely not. The vault needs to be slashed 12 times in order to divide those 12 points efficiently, and this needs to be done multiple times...
Subordinate 2: However, I have heard that you are not required to use an 'efficient method' to divide the points, but rather some other strange method?
Martin: You are right, the key to this strange method is 'Quantum'... Have you heard of Optimization?

All we have is this. All the best in finding the correct replacement for the given function.

**Static noise
```

The encryption scheme used here is same as that used for the shellcodes above

Sample implementation of QAOA in qiskit is given as:

```python!
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

# Number of nodes in the graph
n = 12

G = nx.Graph()
G.add_nodes_from(np.arange(0, n, 1))

elist = [[0, 2, 16.0], [0, 4, 20.0], [0, 6, 17.0], [0, 8, 16.0], [0, 10, 18.0], [1, 0, 13.0], [1, 3, 18.0], [1, 5, 12.0], [1, 7, 14.0], [1, 9, 17.0], [1, 11, 11.0], [2, 1, 20.0], [2, 4, 15.0], [2, 6, 13.0], [2, 8, 19.0], [2, 10, 11.0], [3, 2, 5.0], [3, 5, 13.0], [3, 7, 8.0], [3, 9, 10.0], [3, 11, 14.0], [4, 3, 12.0], [4, 6, 13.0], [4, 8, 8.0], [4, 10, 11.0], [5, 4, 17.0], [5, 7, 13.0], [5, 9, 8.0], [5, 11, 17.0], [6, 5, 6.0], [6, 8, 13.0], [6, 10, 3.0], [7, 6, 7.0], [7, 9, 15.0], [7, 11, 8.0], [8, 7, 10.0], [8, 10, 13.0], [9, 8, 19.0], [9, 11, 19.0], [10, 9, 11.0], [11, 10, 12.0]]




# Add weighted edges to the graph
G.add_weighted_edges_from(elist)

# Set node colors
colors = ["r" for node in G.nodes()]

# Calculate node positions using a spring layout
pos = nx.spring_layout(G)

def draw_graph(G, colors, pos):
    default_axes = plt.axes(frameon=True)
    nx.draw_networkx(G, node_color=colors, node_size=600, alpha=0.8, ax=default_axes, pos=pos)
    edge_labels = nx.get_edge_attributes(G, "weight")
    nx.draw_networkx_edge_labels(G, pos=pos, edge_labels=edge_labels)

# Draw the graph
draw_graph(G, colors, pos)

# Computing the weight matrix from the random graph
w = np.zeros([n, n])
for i in range(n):
    for j in range(n):
        temp = G.get_edge_data(i, j, default=0)
        if temp != 0:
            w[i, j] = temp["weight"]
print(w)

max_cut = Maxcut(w)
qp = max_cut.to_quadratic_program()
print(qp.prettyprint())

qubitOp, offset = qp.to_ising()
print("Offset:", offset)
print("Ising Hamiltonian:")
print(str(qubitOp))

algorithm_globals.random_seed = 123
seed = 10598
# construct SamplingVQE
optimizer = SPSA(maxiter=300)
ry = TwoLocal(qubitOp.num_qubits, "ry", "cz", reps=5, entanglement="linear")
vqe = SamplingVQE(sampler=Sampler(), ansatz=ry, optimizer=optimizer)

# run SamplingVQE
result = vqe.compute_minimum_eigenvalue(qubitOp)

# print results
x = max_cut.sample_most_likely(result.eigenstate)
print("energy:", result.eigenvalue.real)
print("time:", result.optimizer_time)
print("max-cut objective:", result.eigenvalue.real + offset)
print("solution:", x)
print("solution objective:", qp.objective.evaluate(x))

# plot results
colors = ["r" if x[i] == 0 else "c" for i in range(n)]
draw_graph(G, colors, pos)

```


Sample output:

```
energy: -91.93458817618827
time: 143.12175130844116
max-cut objective: -359.4345881761883
solution: [1 0 1 1 0 1 0 1 0 1 1 0]
solution objective: 362.0
```

Here the elist array is the array containing the 41 pairs of points and their associated weights

While calculating weights in our binary, we multiple each weight (distance between 2 points) with a constant value 10 and then round it off 

`solution: [1 0 1 1 0 1 0 1 0 1 1 0]` is the result, 12 bits

Similarly, for Katanaverse 1.0, the brute force method is utilised to divide the points.

<br>

## #6 Formatting the result

The result is then divided into 2 parts of 6 bits each

Each 6 bits is appended by a 01 (or addition of 64 in decimal to the given bits) at the start and made into a proper 8 bits, thus giving out 2 bytes for each solution

For Katanaverse 0.0, QAOA is performed 24 times and thus there are 24*2 bytes at the end

```clike!
qaoa(elist, &res_bits); 
        fin_val[i/6] =  64 + res_bits[0]*1 + res_bits[1]*2 + res_bits[2]*4 + res_bits[3]*8 + res_bits[4]*16 + res_bits[5]*32;
        fin_val[i/6+1] =  64 + res_bits[6]*1 + res_bits[7]*2 + res_bits[8]*4 + res_bits[9]*8 + res_bits[10]*16 + res_bits[11]*32;
```

fin_val holds the final 48 bytes

<br>

### #7 AES Encryption

These 48 bytes are then aes encrypted using the aes_key generated earlier and compared against the encrypted bytes obtained on performing the entire process with the correct flag as input

```clike!
    uint16_t inputSize = sizeof(fin_val)/sizeof(fin_val[0]); // Calculate input size
	uint8_t padding = 16 - (inputSize % 16); // Calculate padding size
    uint32_t paddedSize = inputSize + padding; // Calculate new size with padding
    uint8_t *paddedIn = (uint8_t*)malloc(paddedSize); // Allocate memory for padded input

    if (!paddedIn) {
        // Handle memory allocation failure
        return 1;
    }

    memcpy(paddedIn, fin_val, inputSize); // Copy the original input to the new padded input array

    // Apply PKCS#7 padding
    memset(paddedIn + inputSize, padding, padding);

    uint8_t out[32];
    uint8_t *w; 

	w = aes_init(sizeof(aes_key));
    aes_key_expansion(aes_key, w);

    aes_cipher_multiple_blocks(paddedIn, out , w, paddedSize);

    for(int i = 0; i<64;i++)
    {
        if(out[i] != unk_0[i])
        {
            printf("Nope, that's a wrong path");
            exit(0);
        }
    }

    printf("That's correct!");
```

Also, note that the source code of the AES encryption can be found [here](https://github.com/dhuertas/AES).

<br>
<br>

-----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define stackSize 0x1000 
#define mem_size 0x1000

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}

void b64_encode(char *clrstr, char *b64dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}

struct Registers {
    int S0;
    int S1;
    int S2;
    int S3;
    int S4;
    int S5;
    int S6;
    int S7;
    int PC; 
    int SP; 
    int BP; 
    int MP;
    int EF;
    int ZF;
};

char* read_file(char* filename)
{
    FILE *filePtr;
    filePtr = fopen(filename, "rb");

    if (filePtr == NULL) {
        printf("Error opening the file");
        return NULL;
    }

    fseek(filePtr, 0L, SEEK_END); 
    long int fileSize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);

    char *fileContent = (char *)malloc((fileSize + 1) * sizeof(char));

    fread(fileContent, sizeof(char), fileSize, filePtr);
    fileContent[fileSize] = '\0';
    fclose(filePtr);

    return fileContent;
}


int assign_value(int regist, int value, struct Registers *reg)
{
    switch(regist)
    {
        case 700:
            reg->S0 = value;
            break;
        case 701:
            reg->S1 = value;
            break;
        case 702:
            reg->S2 = value;
            break;
        case 703:
            reg->S3 = value;
            break;
        case 704: 
            reg->S4 = value;
            break;
        case 705:
            reg->S5 = value;
            break;
        case 706:
            reg->S6 = value;
            break;
        case 707:
            reg->S7 = value;
            break;
        case 708:
            reg->MP = value;
            break;
        default:
            printf("Invalid choice of register");
            exit(0);
    }
}

int fetch_value(int regist, struct Registers *reg)
{
    switch(regist)
    {
        case 700:
            return reg->S0;
            break;
        case 701:
            return reg->S1;
            break;
        case 702:
            return reg->S2;
            break;
        case 703:
            return reg->S3;
            break;
        case 704: 
            return reg->S4;
            break;
        case 705:
            return reg->S5;
            break;
        case 706:
            return reg->S6;
            break;
        case 707:
            return reg->S7;
            break;
        case 708:
            return reg->MP;
            break;
        default:
            printf("Invalid choice of register");
            exit(0);
    }
}

int instruction_set(int opcode, struct Registers *reg, char* Code, uint8_t ** Stack, uint8_t **memory)
{
    switch(opcode)
    {
        case 1200: //MOVR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int val = fetch_value(reg_ib, reg);
            assign_value(reg_ib, 0, reg);
            assign_value(reg_ia, val, reg);
            reg->PC = reg->PC + 10;
            break;
        }
        case 1201: // COPI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            assign_value(reg_i, val0, reg); 
            reg->PC = reg->PC+8; 
            break;
        }
        case 1202: //COPR
        {   
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int val = fetch_value(reg_ib, reg);
            assign_value(reg_ia, val, reg);
            reg->PC = reg->PC+10;
            break;
        }
        case 1203: // ADDI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a + val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1204: // ADDR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a + reg_b, reg);
            reg->PC = reg->PC + 10;
            break;            
        }
        case 1205: // SUBI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a - val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1206: // SUBR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a - reg_b, reg);
            reg->PC = reg->PC + 10;
            break;           
        }
        case 1207: // XORI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a ^ val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1208: // XORR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a ^ reg_b, reg);
            reg->PC = reg->PC + 10;
            break;            
        }
        case 1209: //DIVI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a / val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1210: //DIVR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a / reg_b, reg);
            reg->PC = reg->PC + 10;
            break;             
        }
        case 1211: //MULI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a * val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1212: //MULR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a * reg_b, reg);
            reg->PC = reg->PC + 10;
            break;           
        }
        case 1213: //POPA
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            int val = (*Stack)[reg->SP];
            assign_value(reg_i, val, reg);
            reg->SP = reg->SP + 1;
            reg->PC = reg->PC + 7;
            break;
        }
        case 1214: //PUSH
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            reg->SP = reg->SP - 1;
            int val = fetch_value(reg_i, reg);
            (*Stack)[reg->SP] = val;
            reg->PC = reg->PC + 7;
            break;
        }
        case 1215: //LOAD
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            uint8_t a = (*memory)[reg->MP + val0];
            assign_value(reg_i, a, reg);
            reg->PC += 8;
            break;
        }
        case 1216: // STOR
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            int a = fetch_value(reg_i, reg);
            (*memory)[reg->MP] = a;
            reg->MP += 1;
            reg->PC = reg->PC + 7;
            break;
        }
        case 1217: //CALL
        {
            char len[2] = {Code[reg->PC + 4], '\0'};
            char fun_name[len[0]];
            for(int j=0;j<len[0];j++)
            {
                fun_name[j] = Code[reg->PC+5+j];
            }

            void (*fun_name_ptr)() = (void (*)())fun_name;
            fun_name_ptr();
            reg->PC = reg->PC + 5 + len[0];
            break;
        }
        case 1218: //INCR
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a+1, reg);
            reg->PC = reg->PC + 7;
            break;
        }   
        case 1219: //DECR
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a-1, reg);
            reg->PC = reg->PC + 7;
            break;
        }
        case 1220: //LD_D
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            uint8_t a = (*memory)[reg->MP + val0];
            (*memory)[reg->MP + val0] = 0;
            assign_value(reg_i, a, reg);

            reg->PC += 8;
            break;

        }
        case 1221: //SHRI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a >> val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1222: //SHRR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int rega = fetch_value(reg_ia, reg);
            int regb = fetch_value(reg_ib, reg);
            assign_value(reg_ia, rega >> regb, reg);
            reg->PC = reg->PC + 10;
            break;    
        }
        case 1223: //SHLI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a << val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1224: //SHLR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int rega = fetch_value(reg_ia, reg);
            int regb = fetch_value(reg_ib, reg);
            assign_value(reg_ia, rega << regb, reg);
            reg->PC = reg->PC + 10;
            break;    
        }
        case 1225: //PRIM
        {
            char len[2] = {Code[reg->PC + 4], '\0'};
            char message[len[0]];
            for(int j=0;j<len[0];j++)
            {
                message[j] = Code[reg->PC+5+j];
            }
            printf("%s\n", message);
            reg->PC = reg->PC + 5 + len[0];
            break;
        }
        case 1226: //PRIR
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            int a = fetch_value(reg_i, reg);
            printf("%d\n", a);
            reg->PC = reg->PC + 7;
            break;
        }
        case 1227: //COPS
        {
            int val0;
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            if(val[0] == 0x24)
            {
                val0 = 10;
            }
            else {
                val0 = atoi(val);
            }            
            uint8_t a = (*Stack)[reg->SP + val0];
            assign_value(reg_i, a, reg);
            reg->PC += 8;
            break;
        }
        case 1228: //JZD
        {
            uint8_t val = Code[reg->PC + 4];
            if(reg->ZF == 1)
            {
                reg->PC = reg->PC + val;
            }
            else{
                reg->PC = reg->PC + 5;
            }
            break;
        }
        case 1229: //JNZD
        {
            uint8_t val = Code[reg->PC + 4];
            if(reg->ZF == 0)
            {
                reg->PC = reg->PC + val;
            }
            else
            {
                reg->PC = reg->PC + 5;
            }
            break;
        }
        case 1230: //JMPD
        {
            uint8_t val = Code[reg->PC + 4];
            reg->PC = reg->PC + val;
            break;
        }
        case 1231: //SCAN
        {
            char val[2] = {Code[reg->PC + 4], '\0'};
            char reg_s[4] = {Code[reg->PC + 5], Code[reg->PC + 6], Code[reg->PC + 7], '\0'};
            int reg_i = atoi(reg_s);

            switch(val[0])
            {
                case 105:
                {
                    int a;
                    scanf("%d", &a); 
                    assign_value(reg_i, a, reg);
                    reg->PC = reg->PC + 8;
                    break;
                }
                case 99:
                {
                    char c;
                    scanf("%c", &c);
                    (*memory)[reg->MP++] = c;
                    reg->PC = reg->PC + 5;
                    break;
                }
                case 115:
                {
                    char flag[100];
                    fgets(flag, 100, stdin);
                    char cont[100] = {0};
                    char decont[100];
                    int i =8;
                    while(flag[i] != '}')
                    {
                        cont[i-8] = flag[i];
                        i++;
                    }
                    
                    b64_encode(cont, decont);
                    char input[] = "bi0sCTF{";
                    strcat(input, decont);
                    strcat(input, "}");
                    int inp_len = strlen(input);

                    for(int j=0;j<inp_len;j++)
                    {
                        (*memory)[reg->MP++] = input[j];
                    }
                    reg->S0 = inp_len;
                    reg->PC = reg->PC + 5;
                    break;
                }
                default:
                {
                    printf("Invalid choice of scan");
                    exit(0);
                }
            }
            break;
        }
        case 1232: //CMPI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            if(val0 == a)
            {
                reg->ZF = 1;
            }
            else{
                reg->ZF = 0;
            }
            reg->PC = reg->PC + 8;
            break;
        }
        case 1233: //CMPR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int rega = fetch_value(reg_ia, reg);
            int regb = fetch_value(reg_ib, reg);
            if(rega == regb)
            {
                reg->ZF = 1;
            }
            else{
                reg->ZF = 0;
            }
            reg->PC = reg->PC + 10;
            break;
        }
        case 1234: //EXIT
        {
            reg->EF = 1;
            break;
        }
        case 1235: //ANDI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a & val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1236: //ANDR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a & reg_b, reg);
            reg->PC = reg->PC + 10;
            break;            
        }
        case 1237: //  ORRI
        {
            char reg_s[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            int reg_i = atoi(reg_s);
            char val[2] = {Code[reg->PC + 7], '\0'};
            int val0 = atoi(val);
            int a = fetch_value(reg_i, reg);
            assign_value(reg_i, a | val0, reg);
            reg->PC = reg->PC + 8;
            break;
        }
        case 1238: //ORRR
        {
            char reg_sa[4] = {Code[reg->PC + 4], Code[reg->PC + 5], Code[reg->PC + 6], '\0'};
            char reg_sb[4] = {Code[reg->PC + 7], Code[reg->PC + 8], Code[reg->PC + 9], '\0'};
            int reg_ia = atoi(reg_sa);
            int reg_ib = atoi(reg_sb);
            int reg_a = fetch_value(reg_ia, reg);
            int reg_b = fetch_value(reg_ib, reg);
            assign_value(reg_ia, reg_a | reg_b, reg);
            reg->PC = reg->PC + 10;
            break;            
        }
        case 1239: //JZU
        {
            uint8_t val = Code[reg->PC + 4];
            if(reg->ZF == 1)
            {
                reg->PC = reg->PC - val;
            }
            else{
                reg->PC = reg->PC + 5;
            }
            break;
        }
        case 1240: //JNZU
        {
            uint8_t val = Code[reg->PC + 4];
            if(reg->ZF == 0)
            {
                reg->PC = reg->PC - val;
            }
            else
            {
                reg->PC = reg->PC + 5;
            }
            break;
        }
        case 1241: //JMPU
        {
            uint8_t val = Code[reg->PC + 4];
            reg->PC = reg->PC - val;
            break;
        }
        case 1242: //JNZ
        {
            uint8_t val = Code[reg->PC + 4];    
            if(reg->ZF == 0)
            {
                reg->PC = val;
            }
            else
            {
                reg->PC = reg->PC + 5;
            }
            break;
        }
        case 1243: //JZ
        {
            uint8_t val = Code[reg->PC + 4];
            if(reg->ZF == 1)
            {
                reg->PC = val;
            }
            else{
                reg->PC = reg->PC + 5;
            }
            break;
        }
        default:
        {
            printf("Invalid choice of instruction");
            exit(0);
        }

    }
}

int main(int argc, char **argv) {

    if(argc != 2)
    {
        printf("Provide the code dump");
        exit(0);
    }
    char* fileName = argv[1]; 

    struct Registers reg;
    uint8_t *stack = (uint8_t *)malloc(stackSize * sizeof(uint8_t));
    uint8_t *memory = (uint8_t *)malloc(mem_size * sizeof(uint8_t));
    char* Code = read_file(fileName);

    reg.SP = stackSize; 
    reg.BP = stackSize;
    reg.EF = 0;
    reg.ZF = 0;
    reg.MP = 0;
    reg.PC = 0;

    while(reg.EF != 1)
    {
        char instr[5] = {Code[reg.PC], Code[reg.PC + 1], Code[reg.PC + 2], Code[reg.PC + 3], '\0'};
        int inst_i = atoi(instr);
        instruction_set(inst_i, &reg, Code, &stack, &memory);
    }

    int inp_len = reg.S0;
    int classified[(inp_len-1)*6][8];
    
    int c = 0;
    for(int i =0;i<(inp_len-1)*6;i++)
    {
        for(int j =0;j<8;j++)
        {
            classified[i][j] = *(memory + c);
            c += 1;
        }
    }

    return 0;
}

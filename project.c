#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    int Z = 0;
    switch (ALUControl)
    {
    case 000:
        //add
        Z = A + B;
        break;
     case 001:
        //subtract
        Z = A - B;
        break;
    case 010:
        //if less than signed
        Z = (signed )A < (signed) B;
        break;
    case 011:
        //if less than unsigned
        Z = A < B;
        break;
    case 100:
        //AND
        Z = A && B;
        break;
    case 101:
        //OR
        Z = A || B;
        break;
    case 110:
        //shift left
        Z = B << 16;
        break;
    case 111:
        //NOT AS
        Z != A;
        break;
    default:
        break;
    }

    if(Z == 0) {
        Zero = "1";
    } else {
        Zero = "0";
    }

    *ALUresult = Z;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    //check if word-aligned
    if ((PC % 4) != 0) {
        return 1;
    }

    //assign instruction
    *instruction = Mem[PC >> 2];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    unsigned r = 0;
    *op = instruction & 0xfc000000; // bits [31-26]
    *r1 = (instruction & 0x3e000000)>> 21; // bits [25-21]
    *r2 = (instruction & 0x001f00000) >> 16; // bits [20-16]
    *r3 = (instruction & 0x0000f800) >> 11; // bits [15-11]
    *funct = instruction & 0x0000003f; // bits [5-0]
    *offset = instruction & 0x0000ffff; // bits [15-0]
    *jsec = instruction & 0x03ffffff; // bits [25-0]
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    unsigned extend = offset & 0x00008000;

    //if negative
    if(extend > 1) {
        *extended_value = extend |  0xffff0000;
    } else {
        *extended_value = offset;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}


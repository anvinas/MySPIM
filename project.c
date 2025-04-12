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
        Z = ~A;
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
    switch(op){
        case 0: //r-type
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;
        case 8: //add immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 35: //load word
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 43: //store word
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;  
        case 15: //load upper immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break; 
        case 4: //branch on equal
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        case 10: //set less than immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 11: //set less than immediate unsigned
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 2: //jump
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 0;
            break;
        default:
            return 1;
    }

    return 0;
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
    unsigned operand2 = (ALUSrc == 1) ? extended_value : data2;

    if (ALUOp == 7) {
        switch (funct) {
            case 32: //add
                ALUOp = 0;
                break;
            case 34: //subtract
                ALUOp = 1;
                break;
            case 42: //set less than (signed)
                ALUOp = 2;
                break;
            case 43: //set less than (unsigned)
                ALUOp = 3;
                break;
            case 36: //and
                ALUOp = 4;
                break;
            case 37: //or
                ALUOp = 5;
                break;
            case 4: //shift left logical 
                ALUOp = 6;
                break;
            case 39: //nor
                ALUOp = 7;
                break;
            default:
                return 1;
        }
    }

    ALU(data1, operand2, ALUOp, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    //halt
    if(ALUresult % 4 != 0) {
        return 1;
    }

    if(MemRead) { //if read
       *memdata = Mem[ALUresult >> 2];
    } else if (MemWrite) {//if write 
        Mem[ALUresult >> 2] = data2;
    }

    //when not halt
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    //if write
    if (RegWrite == 0) return;

    if(MemtoReg == 1 && RegDst == 0) { //r2 to mem
        Reg[r2] = memdata;
    } else if (MemtoReg == 1 && RegDst == 1){ //r3 to mem
        Reg[r3] = memdata;
    } else if (MemtoReg == 0 && RegDst == 0) {// r2 to result
        Reg[r2] = ALUresult;
    } else if (MemtoReg == 0 && RegDst == 1) {//r3 to result
        Reg[r3] = ALUresult;
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //increment PC
    *PC += 4;

    if(Branch && Zero) { //if branch
        *PC += extended_value << 2;
    } else if(Jump == 1) { //if jump
        *PC = (*PC & 0xF0000000) | (jsec << 2);
    }
}




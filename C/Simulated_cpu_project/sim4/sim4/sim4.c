#include "sim4.h"

void extract_instructionFields(WORD instruction, InstructionFields *fieldsOut){
    //bit shift and mask instruction to get the 
    //individual values expected by instuctionFileds
    //opcode - 6 bits 31 - 26
    //rs -  5 bits 25 - 21
    //rt - 5 bits 20 - 16
    //rd - 5 bits 15 - 11
    //shamt - 5 bits 10 - 6
    //funct - 6 bits 5 - 0
    //imm16 - 16 bits 15 - 0
    //imm32 - 32 (imm16 sign extended)
    //address - 26 bit 25 - 0

    fieldsOut -> opcode = (instruction >> 26) & 0x3f;
    fieldsOut -> rs = (instruction >> 21) & 0x1f;
    fieldsOut -> rt = (instruction >> 16) & 0x1f;
    fieldsOut -> rd = (instruction >> 11) & 0x1f;
    fieldsOut -> shamt = (instruction >> 6) & 0x1f;
    fieldsOut -> funct = instruction & 0x3f;
    fieldsOut -> imm16 = instruction & 0xffff;
    fieldsOut -> imm32 = signExtend16to32(fieldsOut -> imm16);
    fieldsOut -> address = instruction & 0x3ffffff;
}

int  fill_CPUControl(InstructionFields *fields, CPUControl *controlOut){
    //CPUControl must have the following fields set:
    //ALUsrc
    //ALU.op
    //ALU.bNegate
    //memRead
    //memWrite
    //memToReg
    //regDst
    //regWrite
    //branch
    //jump

    if (fields-> opcode == 0){
        //if opcode 0 R-Format check funct 
        if (fields -> funct == 32){
        //if opcode 0 and funct 32 - add
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 2;
            controlOut -> ALU.bNegate = 0;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;

            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0; 

        } else if (fields -> funct == 33){
        //if opcode 0 and funct 33 - addu    
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 2;
            controlOut -> ALU.bNegate = 0;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;

            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0; 

        } else if (fields -> funct == 34){
        //if opcode 0 and funct 34 - sub    
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 2;
            controlOut -> ALU.bNegate = 1;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;

        } else if (fields -> funct == 35){
        //if opcode 0 and funct 35 - subu
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 2;
            controlOut -> ALU.bNegate = 1;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;

        } else if (fields -> funct == 36){
        //if opcode 0 and funct 36 - and
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 0;
            controlOut -> ALU.bNegate = 0;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;
        } else if (fields -> funct == 37){
        //if opcode 0 and funct 37 - or
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 1;
            controlOut -> ALU.bNegate = 0;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;
        } else if (fields -> funct == 38){
        //if opcode 0 and funct 38 - xor
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 4;
            controlOut -> ALU.bNegate = 0;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;
        } else if (fields -> funct == 42){
        //if opcode 0 and funct 42 - slt
            controlOut -> ALUsrc = 0;

            controlOut -> ALU.op = 3;
            controlOut -> ALU.bNegate = 1;

            controlOut -> memRead = 0;
            controlOut -> memWrite = 0;
            controlOut -> memToReg = 0;
            
            controlOut -> regDst = 1;
            controlOut -> regWrite = 1;

            controlOut -> branch = 0;
            controlOut -> jump = 0;
        } else{
            return 0;
        }

    } else if (fields -> opcode ==  2){
    //if opcode 2 - jump
        controlOut -> ALUsrc = 0;

        controlOut -> ALU.op = 0;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 0;

        controlOut -> branch = 0;
        controlOut -> jump = 1;

    } else if (fields -> opcode == 4){
    //if opcode 4 - beq
        controlOut -> ALUsrc = 0;

        controlOut -> ALU.op = 2;
        controlOut -> ALU.bNegate = 1;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 0;

        controlOut -> branch = 1;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 5){
    //if opcode 5 - bne
        controlOut -> ALUsrc = 0;

        controlOut -> ALU.op = 6;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 0;

        controlOut -> branch = 1;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 8){
    //if opcode 8 - addi 
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 2;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 9){
    //if opcode 9 - addiu 
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 2;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 10){
    //if opcode 10 - slti
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 3;
        controlOut -> ALU.bNegate = 1;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;
    } else if (fields -> opcode == 12){
    //if opcode 12 - andi
        controlOut -> ALUsrc = 2;

        controlOut -> ALU.op = 0;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 13){
    //if opcode 13 - ori
        controlOut -> ALUsrc = 2;

        controlOut -> ALU.op = 1;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 14){
    //if opcode 14 - xori
        controlOut -> ALUsrc = 2;

        controlOut -> ALU.op = 4;
        controlOut -> ALU.bNegate =0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 15){
    //if opcode 15 - lui
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 5;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 35){
    //if opcode 35 - lw 
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 2;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 1;
        controlOut -> memWrite = 0;
        controlOut -> memToReg = 1;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 1;

        controlOut -> branch = 0;
        controlOut -> jump = 0;

    } else if (fields -> opcode == 43){
    //if opcode 43 - sw 
        controlOut -> ALUsrc = 1;

        controlOut -> ALU.op = 2;
        controlOut -> ALU.bNegate = 0;

        controlOut -> memRead = 0;
        controlOut -> memWrite = 1;
        controlOut -> memToReg = 0;

        controlOut -> regDst = 0;
        controlOut -> regWrite = 0;

        controlOut -> branch = 0;
        controlOut -> jump = 0;
    } else {
        return 0;
    }
}

WORD getInstruction(WORD curPC, WORD *instructionMemory){
    return instructionMemory[(curPC - 0)/4];
}

WORD getALUinput1(CPUControl *controlIn, InstructionFields *fieldsIn, WORD rsVal,
                  WORD rtVal, WORD reg32, WORD reg33, WORD oldPC){
    return rsVal;
}

WORD getALUinput2(CPUControl *controlIn, InstructionFields *fieldsIn, WORD rsVal, 
                  WORD rtVal, WORD reg32, WORD reg33, WORD oldPC){
    if (controlIn->ALUsrc == 0){
        return rtVal;
    } else if (controlIn -> ALUsrc == 2){
        return fieldsIn -> imm16;
    } else {
        return fieldsIn -> imm32;
    }

}

void execute_ALU(CPUControl *controlIn, WORD input1, WORD input2, 
                 ALUResult *aluResultOut){
    
    if (controlIn -> ALU.op == 0){
        aluResultOut -> result = input1 & input2;
    } else if (controlIn -> ALU.op == 1){
        aluResultOut -> result = input1 | input2;
    } else if (controlIn -> ALU.op == 2){
        if (controlIn -> ALU.bNegate == 1){
            aluResultOut -> result = input1 - input2;
        } else {
            aluResultOut -> result = input1 + input2;
        }
    } else if (controlIn -> ALU.op == 3){
        aluResultOut -> result = input1 < input2;
    } else if (controlIn -> ALU.op == 4){
        aluResultOut -> result = input1 ^ input2;
    } else if (controlIn -> ALU.op == 5){
        aluResultOut -> result = input2 << 16;
    } else if (controlIn -> ALU.op == 6){
        aluResultOut -> result = input1 == input2;
    } 
    
    if (aluResultOut -> result == 0){
        aluResultOut -> zero = 1;
    } else {
        aluResultOut -> zero = 0;
    }
}

void execute_MEM(CPUControl *controlIn, ALUResult *aluResultIn, WORD rsVal, 
                 WORD rtVal, WORD *memory, MemResult *resultOut){
    if (controlIn -> memRead == 1){
        resultOut -> readVal = memory[(aluResultIn -> result)/4];
    } else if (controlIn -> memWrite == 1){
        memory[(aluResultIn -> result)/4] = rtVal;
    }
}

WORD getNextPC(InstructionFields *fields, CPUControl *controlIn, int aluZero,
               WORD rsVal, WORD rtVal, WORD oldPC){
    
    WORD newPC = oldPC + 4;

    if (controlIn -> jump == 1){
        int newAddress = (fields -> address) << 2;
        newPC = (oldPC & 0xf0000000) | (newAddress);
    } else if (controlIn -> jump == 0 && controlIn -> branch == 1 && aluZero == 1){
        int newAddress = ((fields -> imm32) << 2) + newPC;
        newPC = newAddress;
    }

    return newPC;

}

void execute_updateRegs(InstructionFields *fields, CPUControl *controlIn, 
                        ALUResult *aluResultIn, MemResult *memResultIn, WORD *regs){
    
    if (controlIn -> regWrite == 0){
        return;
    }
    if (controlIn -> memToReg == 1){
        if (controlIn -> regDst == 0){
            regs[fields -> rt] = memResultIn -> readVal;
        } else {
            regs[fields -> rd] = memResultIn -> readVal;
        }
    } else {
        if (controlIn -> regDst == 0){
            regs[fields -> rt] = aluResultIn -> result;
        } else {
            regs[fields -> rd] = aluResultIn -> result;
        }
    }
}
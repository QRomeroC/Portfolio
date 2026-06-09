#include "sim5.h"

//Author: Cesar D. Quihuis-Romero
//Purpose: sim5.c will simulate a pipelined processor. It will build
//on the cpu design and functionallity utilized in sim4.c (milestones 1 and 2).

//Method: sim5.c will utilized additional structs not found in sim4.c. These 
//structs will act as pipeline registers. sim5.h outlines the following pipeline
//structs which represent there respective registers:
//ID_EX, EX_MEM, MEM_WB.
//Additionally, no global control struct will be implamented (as found in sim4.c);
//instead each pipeline register struct will be responsable for containing all 
//information saved from one clock cycle to subsequent cycles.

void extract_instructionFields(WORD instruction, InstructionFields *fieldsOut){
//extract_instructionFields takes a 32 bit WORD - instruction and bit shift and
//masks to extract the bits that represent:
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
}//end of extract_instructionFields

int IDtoIF_get_stall(InstructionFields *fields, ID_EX *old_idex, EX_MEM *old_exmem){
//IDtoIF_get_stall looks at the current *fields struct and determins if a 
//data hazard exists which requires a stall. It will use the rs/rt destinations
//of the current (*fields) and the rs/rt destinations found in either old_idex
//or old_exMem and if a conflect exists return 1 indicating a stall is required
//else it will return 0 and no stall will be inserted. 

    //opcode 43 = sw 
    if (fields -> opcode == 43){
        //IDEX_destReg will be old_idex rt or rd based on its regDst
        int IDEX_destReg;
        if (old_idex -> regDst == 0){
            IDEX_destReg = old_idex -> rt;
        } else {
            IDEX_destReg = old_idex -> rd;
        }

        //check if instruct one behind is writting to the same register as current instruction
        //if so then we need to return 0 prior to checking the state of the instruction
        //two behind. (handles case if both instructions are updating the same reg and
        //that reg is being read from in current sw instuction).

        if (old_idex -> regWrite == 1 && (IDEX_destReg == fields -> rt)){
            //return 0 as hazard can be resolved with fowarding
            return 0;

        //check if instruction two instructions behind is writting to same reg as curr
        //the instruction one before curr must not be updating a register or the reg 
        //it is updating must not be the same as curr instuctions rt.

        } else if (old_exmem -> regWrite == 1 && (old_exmem -> writeReg == fields -> rt)){
            //return 1 as this hazard cannot be resolved with fowarding and must stall
            return 1;
        } 
    }//end of SW hazard midigation

    //if instruction one behind is LW
    if (old_idex -> memRead == 1){
        //if current instruction is R-Format
        if (fields -> opcode == 0){
            //check one behind rt against curr instruction rt and rs
            if (old_idex -> rt == fields -> rt || old_idex -> rt == fields -> rs){
                //return 1 as this hazard is a valid hazard and cannot be resolved by fowarding
                return 1;
            } 
        //if curr instruction is not R-Format 
        } else if (fields -> opcode != 0){
            //just check one behind rt against curr instruction rs
            if (old_idex -> rt == fields -> rs){
                //return 1 since cannot foward and must stall
                return 1;
            } 
        }
    }//end of LW hazard midigation

    //if no stall is caught by if/else ahead then default return 0
    return 0;
}//end of IDtoIF_get_stall

int IDtoIF_get_branchControl(InstructionFields *fields, WORD rsVal, WORD rtVal){
//IDtoIF_get_brachControl will look at the current *fields -> opcode and determine
//if a branch is required 
    
    //opcode 0 = R-Format (no branch/jump)
    if (fields -> opcode == 0){
        return 0;
    //opcode 4 = beq (check if both values are equal)
    } else if (fields -> opcode == 4 && rsVal == rtVal){
        return 1;
    //opcode 5 = bne (check if both values are not equal)
    } else if (fields -> opcode == 5 && rsVal != rtVal){
        return 1;
    //opcode 2 = jump (uncoditinal)
    } else if (fields -> opcode == 2) {
        return 2;
    }
    //if non of the above return 0 - opcode not reconized 
    return 0;
}//end of IDtoIF_get_branchControl

WORD calc_branchAddr(WORD pcPlus4, InstructionFields *fields){
//new branch address is pc+4 + the immediate 32 fields shifted left
    return pcPlus4 + ((fields -> imm32 ) << 2);
}//end of calc_brachAddr

WORD calc_jumpAddr(WORD pcPlus4, InstructionFields *fields){
//new jump address is nibble of pc+4 with the 26 bit address field shift left making it 28 bits
    return (pcPlus4 & 0xf0000000) | ((fields -> address) << 2);
}//end of calc_jumpAddr

int execute_ID(int IDStall, InstructionFields *fieldsIn, WORD pcPlus4, WORD rsVal,
               WORD rtVal, ID_EX *new_idex){
    //default values setting all to 0 specific combinations of opcode/funct will
    //update these pipeline registers as needed.
    new_idex -> rs = 0;
    new_idex -> rt = 0;
    new_idex -> rd = 0;

    new_idex -> rsVal = 0;
    new_idex -> rtVal = 0;

    new_idex -> imm16 = 0;
    new_idex -> imm32 = 0;

    new_idex -> ALUsrc = 0;

    new_idex -> ALU.op = 0;
    new_idex -> ALU.bNegate = 0;

    new_idex -> memRead = 0;
    new_idex -> memWrite = 0;
    new_idex -> memToReg = 0;

    new_idex -> regDst = 0;
    new_idex -> regWrite = 0;
    //if IDStall == 1 then return 1 and do not further update fields
    if (IDStall == 1){
        return 1;
    } else {
        //value mapping same in all other cases:
        new_idex -> rs = fieldsIn -> rs;
        new_idex -> rt = fieldsIn -> rt;
        new_idex -> rd = fieldsIn -> rd;

        new_idex -> rsVal = rsVal;
        new_idex -> rtVal = rtVal;

        new_idex -> imm16 = fieldsIn -> imm16;
        new_idex -> imm32 = fieldsIn -> imm32;

        //Case specific mapping as follows:
        //opcode 0 indicates R-Format
        if (fieldsIn -> opcode == 0){
            switch (fieldsIn -> funct){
                //case 0 is NOP
                case 0:
                    //set all values to 0 except ALU.op
                    new_idex -> rs = 0;
                    new_idex -> rt = 0;
                    new_idex -> rd = 0;

                    new_idex -> rsVal = 0;
                    new_idex -> rtVal = 0;

                    new_idex -> imm16 = 0;
                    new_idex -> imm32 = 0;

                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 5; //ALU.op = 5 is SLL (NOP)
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 32 = add
                case 32:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 2; //ALU.op = 2 is ADD
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 33 = addu
                case 33:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 2; //ALU.op = 2 is ADD
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 34 = sub
                case 34:
                    new_idex -> rs = fieldsIn -> rs;
                    new_idex -> rt = fieldsIn -> rt;
                    new_idex -> rd = fieldsIn -> rd;

                    new_idex -> rsVal = rsVal;
                    new_idex -> rtVal = rtVal;

                    new_idex -> imm16 = fieldsIn -> imm16;
                    new_idex -> imm32 = fieldsIn -> imm32;

                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 2; //ALU.op = 2 is ADD
                    new_idex -> ALU.bNegate = 1; //negate input 2 for subtraction

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 35 = subu
                case 35:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 2; //ALU.op = 2 is ADD
                    new_idex -> ALU.bNegate = 1; //Negate input 2 for sub

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;
                    return 1;
                //funct 36 = and
                case 36:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 0; // ALU.op = 0 is AND
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 37 = or
                case 37:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 1; //is OR
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 38 = xor
                case 38:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 4; //is XOR
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 39 = nor
                case 39:
                    new_idex -> ALUsrc = 0;
                    new_idex -> ALU.op = 6; //is NOR
                    new_idex -> ALU.bNegate = 0;

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
                //funct 42 = slt
                case 42:
                    new_idex -> ALUsrc = 0;

                    new_idex -> ALU.op = 3; //is less
                    new_idex -> ALU.bNegate = 1; //negate input 2 for compare

                    new_idex -> memRead = 0;
                    new_idex -> memWrite = 0;
                    new_idex -> memToReg = 0;

                    new_idex -> regDst = 1;
                    new_idex -> regWrite = 1;

                    return 1;
            }
        //end of all R-Format instructions - start of I-Format       
        } else if (fieldsIn -> opcode == 2){
        //opcode 2 = jump (unconditional)
        //all values = 0 since jump is calculated by calc jump
            new_idex -> rs = 0;
            new_idex -> rt = 0;
            new_idex -> rd = 0;

            new_idex -> rsVal = 0;
            new_idex -> rtVal = 0;

            new_idex -> imm16 = 0;
            new_idex -> imm32 = 0;

            new_idex -> ALUsrc = 0;

            new_idex -> ALU.op = 0;
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 0;

            return 1;

        } else if (fieldsIn -> opcode == 4){
        //opcode 4 = beq
        //set all values to 0 - branch calculated in calcBranch
            new_idex -> rs = 0;
            new_idex -> rt = 0;
            new_idex -> rd = 0;

            new_idex -> rsVal = 0;
            new_idex -> rtVal = 0;

            new_idex -> imm16 = 0;
            new_idex -> imm32 = 0;

            new_idex -> ALUsrc = 0;

            new_idex -> ALU.op = 0;
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 0;

            return 1;

        } else if (fieldsIn -> opcode == 5){
        //opcode 5 = bne
        //same as BEQ
            new_idex -> rs = 0;
            new_idex -> rt = 0;
            new_idex -> rd = 0;

            new_idex -> rsVal = 0;
            new_idex -> rtVal = 0;

            new_idex -> imm16 = 0;
            new_idex -> imm32 = 0;

            new_idex -> ALUsrc = 0;

            new_idex -> ALU.op = 0;
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 0;

            return 1;

        } else if (fieldsIn -> opcode == 8){
        //opcode 8 = addi
            new_idex -> ALUsrc = 1; //sorce 1 for imm field

            new_idex -> ALU.op = 2; //is ADD
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0; //regDst 0 for I - Format
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 9){
        //opcode 9 = addiu
        //same as addi
            new_idex -> ALUsrc = 1;

            new_idex -> ALU.op = 2;
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 10){
        //opcode 10 = slti
            new_idex -> ALUsrc = 1; //source 1 for imm field

            new_idex -> ALU.op = 3; //is less
            new_idex -> ALU.bNegate = 1; //negate input2 for compare

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0; //regDst reflect I-Format
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 12){
        //opcode 12 = andi
        //Source 2 for all logical imm (input2 will be zero extended imm16)
            new_idex -> ALUsrc = 2;

            new_idex -> ALU.op = 0; //is AND
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 13){
        //opcode 13 = ori
        //same as andi
            new_idex -> ALUsrc = 2;

            new_idex -> ALU.op = 1; //is OR
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 14){
        //opcode 14 = xori
        //same as other logical imms
            new_idex -> ALUsrc = 2;

            new_idex -> ALU.op = 4; // is XOR
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 15){
        //opcode 15 = lui
            new_idex -> ALUsrc = 1;

            new_idex -> ALU.op = 7; //op 7 shifts input 2 << 16 for lui
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 35){
        //opcode 35 = lw
            new_idex -> ALUsrc = 1;

            new_idex -> ALU.op = 2; //is ADD for address calculation
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 1;
            new_idex -> memWrite = 0;
            new_idex -> memToReg = 1;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 1;

            return 1;
        } else if (fieldsIn -> opcode == 43){
        //opcode 43 = sw
            new_idex -> ALUsrc = 1;

            new_idex -> ALU.op = 2; //is ADD for address calculation
            new_idex -> ALU.bNegate = 0;

            new_idex -> memRead = 0;
            new_idex -> memWrite = 1;
            new_idex -> memToReg = 0;

            new_idex -> regDst = 0;
            new_idex -> regWrite = 0;

            return 1;
        } else {
            //if not caputred by cases then return 0 as unreq instruction
            return 0;
        }
    }
}//end of execute_ID

WORD EX_getALUinput1(ID_EX *in, EX_MEM *old_exmem, MEM_WB *old_memWb){
//Get alu input 1 does data forwarding as needed
//case 1 if instruction 1 ahead is writing to reg curr instruction is reading
    if (old_exmem -> regWrite == 1 && old_exmem -> writeReg == in -> rs){
        return old_exmem -> aluResult;
    } 
    //cse 2 if instruction 2 ahead is writing to register
    if (old_memWb -> regWrite == 1){
        //if its writing to the same register curr instruction is reading from
        if (old_memWb -> writeReg == in -> rs){
            //if instruction 2 ahead is writing a value from memory
            if (old_memWb -> memToReg == 1){
                return old_memWb -> memResult;
            //if instuction 2 ahdead is writing a value calculated in ALU
            } else {
                return old_memWb -> aluResult;
            }
        }
    }
    //if no fowarding need use value held in rs reg
    return in -> rsVal;
}//end of EX_getALUinput1

WORD EX_getALUinput2(ID_EX *in, EX_MEM *old_exMem, MEM_WB *old_memWb){
//Get alu input 2 does data fowarding as needed, also uses imm fields as req
//source 1 = use imm32
    if (in -> ALUsrc == 1){
        return in -> imm32;
    //sorce 2 = use imm16
    } else if (in -> ALUsrc == 2){
        return in -> imm16;
    //else determine if fowarding is required
    } else {
        //check instuction one ahead for fowarding
        if(old_exMem -> regWrite == 1 && old_exMem -> writeReg == in -> rt){
            return old_exMem -> aluResult;
        }
        //check if instuction two ahead for fowarding
        if (old_memWb -> regWrite == 1 && old_memWb -> writeReg == in -> rt){
            //if reading from mem foward memory result
            if (old_memWb -> memToReg == 1){
                return old_memWb -> memResult;
            //else foward instruction two ahead's alu result value
            } else {
                return old_memWb -> aluResult;
            }
        }
    }
    //if no fowarding required use rtVal
    return in -> rtVal;
}//end of EX_getALUinput2

void execute_EX(ID_EX *in, WORD input1, WORD input2, EX_MEM *new_exMem){
//execute_EX represents the ALU, executes the given function on input1 and input2
//as retreaved from get_inputs functions (for input1/2)

    //default value propegation - update next bank of pipeline regesters 
    new_exMem -> rt = in -> rt;
    new_exMem -> rtVal = in -> rtVal;

    new_exMem -> memRead = in -> memRead;
    new_exMem -> memWrite = in -> memWrite;
    new_exMem -> memToReg = in -> memToReg;
      new_exMem -> regWrite = in -> regWrite;

    //determin destination register for R-Format vs I-Format
    if (in -> regDst == 0){
        new_exMem -> writeReg = in -> rt;
    } else {
        new_exMem -> writeReg = in -> rd;
    }
    //switch/case to handle diffrent ALU op codes
    switch (in -> ALU.op){
        //case 0 == AND
        case 0:
            new_exMem -> aluResult = input1 & input2;
            break;
        //case 1 == OR
        case 1:
            new_exMem -> aluResult = input1 | input2;
            break;
        //case 2 == ADD
        case 2: 
            //check if add or sub
            if (in -> ALU.bNegate == 0){
                new_exMem -> aluResult = input1 + input2;
            } else {
                new_exMem -> aluResult = input1 - input2;
            }
            break;
        //case 3 == LESS
        case 3:
            new_exMem -> aluResult = input1 < input2;
            break;
        //case 4 == XOR
        case 4:
            new_exMem -> aluResult = input1 ^ input2;
            break;
        //case 5 == SLL (used for NOP)
        case 5:
            new_exMem -> aluResult = input1 << input2;
            break;
        //case 6 == NOR
        case 6:
            new_exMem -> aluResult = ~(input1 | input2);
            break;
        //case 7 == LUI
        case 7:
            new_exMem -> aluResult = input2 << 16;
            break;
    }

}//end of execute_EX


void execute_MEM(EX_MEM *in, MEM_WB *old_memWb, WORD *mem, MEM_WB *new_memwb){
//execute_MEM represents the memory unit - responsible for putting a value
//from a register into memory or reading a value from memory into a register
//handles fowarding as needed

    //default value propgation - one to one copy
    new_memwb -> memToReg = in -> memToReg;
    new_memwb -> aluResult = in -> aluResult;
    new_memwb -> writeReg = in -> writeReg;
    new_memwb -> regWrite = in -> regWrite;

    //default memResult will be zero - specific cases will override as needed
    new_memwb -> memResult = 0;

    //check if were returning a value from memory
    if (in -> memRead == 1){
        new_memwb -> memResult = mem[(in -> aluResult)/4];
    //else check if were writing a value to memory
    } else if (in -> memWrite == 1){
        //first check if we need to update value were are writing to memory
        //from a value generated by the instruction 2 ahead
        if (old_memWb -> regWrite == 1 && old_memWb -> memToReg == 1){
            //if instuction 2 ahead is updating a register (from memory) and the register is 
            //same as curr instruction - foward
            if (in -> rt == old_memWb -> writeReg){
                mem[(in -> aluResult)/4] = old_memWb -> memResult;
            //else use curr rtVal - no fowarding
            } else {
                mem[(in -> aluResult)/4] = in -> rtVal;
            }
        //check if we need to again foward value but not a value pulled from memory
        //indicates instuction 2 ahead is updating register with a value calculated by 
        //ALU not read from memory - still foward but alu result 
        } else if (old_memWb -> regWrite == 1 && old_memWb -> memToReg == 0){
             if (in -> rt == old_memWb -> writeReg){
                mem[(in -> aluResult)/4] = old_memWb -> aluResult;
            //else no fowarding 
            } else {
                mem[(in -> aluResult)/4] = in -> rtVal;    
            }
        //for all other cases where non fowarding memory write is required
        } else {
            mem[(in -> aluResult)/4] = in -> rtVal;
        }
    
    } 
    
}//end of execute_MEM

void execute_WB (MEM_WB *in, WORD *regs){
//execute_WB - updates registers with either result of a mem read or with the ALU result

    //if curr instruction is not writing to a register do nothing
    if (in -> regWrite == 0){
        return;
    //else determine if value came from memory or from ALU calculation
    } else {
        //if curr instuction is writing a value from memory into a register
        if (in -> memToReg == 1){
            regs[in -> writeReg] = in -> memResult;
        //else value is coming from ALU
        } else {
            regs[in -> writeReg] = in -> aluResult;
        }
    }
}//end of execute_WB
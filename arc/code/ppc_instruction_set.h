#ifndef PPC_INSTRUCTION_SET_H

enum ppc_instruction_form
{
    None_Form,
    
    I_Form,
    B_Form,
    SC_Form,
    
    D_Form,
    X_Form,
};

typedef struct ppc_instruction
{
    u16 Opcode;
    char Mnemonic[8];
    
    u16 Form;
    
    u16 ExtendedOpcode;
    
    void (*CustomDecoding)(void *FormatData);
} ppc_instruction;

global_variable ppc_instruction 
InstructionSet[] = 
{
    //{2, "tdi", D_Form}, // ppc64 // TODO: Add extended mnemonics.
    {3, "twi", D_Form, 0, TrapDecoder}, // TODO: Add extended mnemonics.
    
    {7, "mulli", D_Form},
    
    {16, "bc", B_Form},
    {17, "sc", SC_Form},
    {18, "b", I_Form},
    
    {31, "or", X_Form, 444},
    
    {32, "lwz", D_Form},
    {33, "lwzu", D_Form},
    {34, "lbz", D_Form},
    {35, "lbzu", D_Form},
    
    {36, "stw", D_Form},
    {37, "stwu", D_Form},
    {38, "stb", D_Form},
    {39, "stbu", D_Form},
};

#define PPC_INSTRUCTION_SET_H
#endif

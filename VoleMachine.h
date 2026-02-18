
#include <iostream>
#include <string>
#include <vector>

#ifndef _VOLEMACHINE_H
#define _VOLEMACHINE_H

using namespace std;

class Machine;

class Memory {
private:
    string memory[256];

public:
    void setCell(int address, const string& value);
    const string& getCell(int address) const;
};


class Register {
private:
    int memory[16];

public:
    void setCell(int address, int value);

    Register();

    int getCell(int address) const;
};


class ALU {
public:
    int HextoDec(const string& hexVal);
    string decToHex(const int &decimalValue) const;
    bool isValidIndex(int index);
    void add(int idx1, int idx2, int resultIdx, Register& reg);
    void addFloat(int idx1, int idx2, int resultIdx, Register& reg);
};


class CU {
private:
    bool isValidRegisterIndex(int idx) const {
        return (idx >= 0 && idx < 16);
    }
public:
    void load(int idxReg, int idxMem, Register& reg, Memory& memory);
    void load(int idxReg, int value, Register& reg);
    void store(int idxReg, int idxMem, Register& reg, Memory& memory);
    void move(int idxReg1, int idxReg2, Register& reg);
    void jump(int idxReg, int idxMem, Register& reg, int& PC);
    void halt(Machine& machine);

};

class CPU {
private:
    int programCounter;
    string instructionRegister;
    Register reg;
    ALU alu;
    CU cu;
    void fetch(Memory& memory);
    vector<int> decode();
    void execute(Memory& memory, const vector<int>& decodedValues, Machine& machine);

public:
    CPU() : programCounter(0), instructionRegister("") {}
    void runNextStep(Memory& memory, Machine& machine);
    int getProgramCounter() const;
    string getInstructionRegister() const;
    int getRegisterValue(int index) const;
};


class Machine {
private:
    Memory memory;
    CPU processor;
    bool halted = false;

public:
    void loadProgramFile();
    void outputState();

    void runNextStep() {
        processor.runNextStep(memory, *this);
    }

    int getProgramCounter() const {
        return processor.getProgramCounter();
    }

    string getInstructionRegister() const {
        return processor.getInstructionRegister();
    }

    int getRegisterValue(int index) const {
        return processor.getRegisterValue(index);
    }

    bool isHalted() const { return halted; }
    void setHalted(bool value) { halted = value; }
};

#endif

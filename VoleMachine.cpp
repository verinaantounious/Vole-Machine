#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <bitset>
#include <cstdint>
#include "VoleMachine.h"

using namespace std;

//Memory
void Memory::setCell(const int address, const string& value) {
    if (address >= 0 && address < 256) {
        memory[address] = value;
    }
    else {
        cout << "Error: Memory address out of range" << endl;
    }
}

const string& Memory::getCell(const int address) const {
    if (address >= 0 && address < 256) {
        return memory[address];
    }
    else {
        cout << "Error: Memory address out of range" << endl;
        static string emptyString = "";
        return emptyString;
    }
}


//Register
void Register::setCell(const int address, int value) {
    if (address >= 0 && address < 16) {
        memory[address] = value;
    }
    else {
        cout << "Error: Register address out of range" << endl;
    }
}

Register::Register() {
    for (int & i : memory) {
        i = 0;
    }
}

int Register::getCell(int address) const {
    if (address >= 0 && address < 16) {
        return memory[address];
    }
    else {
        cout << "Error: Register address out of range" << endl;
    }
    return 0;
}

//ALU
int ALU::HextoDec(const string& hexVal) {
    return stoi(hexVal, nullptr, 16);
}

string ALU::decToHex(const int& decimalValue) const {
    stringstream ss;
    ss << uppercase << hex << decimalValue;
    string result = ss.str();
    if (result.length() < 2) result = "0" + result;
    return result;
}

bool ALU::isValidIndex(int index) {
    return (index >= 0 && index <= 15);
}


void ALU::add(int idxS, int idxT, int idxR, Register& reg) {
    int hexValue1 = reg.getCell(idxS);
    int hexValue2 = reg.getCell(idxT);

    bitset<8> binValue1(hexValue1);
    bitset<8> binValue2(hexValue2);

    int value1 = (binValue1.to_ulong() & 0x80) ? binValue1.to_ulong() - 256 : binValue1.to_ulong();
    int value2 = (binValue2.to_ulong() & 0x80) ? binValue2.to_ulong() - 256 : binValue2.to_ulong();
    int result = value1 + value2;

    if (result > 127) {
        result -= 256;
    }
    else if (result < -128) {
        result += 256;
    }

    bitset<8> resultBinary(result);
    int resultHex = static_cast<int>(resultBinary.to_ulong());
    reg.setCell(idxR, resultHex);
}


void ALU::addFloat(int idx1, int idx2, int resultIdx, Register& reg) {
    int hexValue1 = reg.getCell(idx1);
    int hexValue2 = reg.getCell(idx2);

    uint8_t value1 = static_cast<uint8_t>(hexValue1);
    uint8_t value2 = static_cast<uint8_t>(hexValue2);

    int sign1 = (value1 >> 7) & 0x1;
    int exponent1 = (value1 >> 4) & 0x7;
    int mantissa1 = value1 & 0xF;

    int sign2 = (value2 >> 7) & 0x1;
    int exponent2 = (value2 >> 4) & 0x7;
    int mantissa2 = value2 & 0xF;

    float decimalValue1 = (sign1 ? -1 : 1) * (1.0f + mantissa1 / 16.0f) * pow(2, exponent1 - 3);
    float decimalValue2 = (sign2 ? -1 : 1) * (1.0f + mantissa2 / 16.0f) * pow(2, exponent2 - 3);

    float resultFloat = decimalValue1 + decimalValue2;

    int signResult = (resultFloat < 0) ? 1 : 0;
    resultFloat = fabs(resultFloat);

    int exponentResult = 0;
    while (resultFloat >= 2.0f) {
        resultFloat /= 2.0f;
        exponentResult++;
    }

    exponentResult += 3;
    int mantissaResult = static_cast<int>((resultFloat - 1.0f) * 16);
    exponentResult = std::min(exponentResult, 7);
    uint8_t resultNormalized = (signResult << 7) | ((exponentResult & 0x7) << 4) | (mantissaResult & 0xF);
    reg.setCell(resultIdx, static_cast<int>(resultNormalized));
}



//CU
void CU::load(int idxReg, int idxMem, Register& reg, Memory& memory) {
    if (isValidRegisterIndex(idxReg)) {
        int value = memory.getCell(idxMem).empty() ? 0 : stoi(memory.getCell(idxMem), nullptr, 16);
        reg.setCell(idxReg, value);
    }
    else {
        cout << "Invalid register index in load operation!" << endl;
    }
}

void CU::load(int idxReg, int value, Register& reg) {
    if (isValidRegisterIndex(idxReg)) {
        reg.setCell(idxReg, value);
    }
    else {
        cout << "Invalid register index in load operation!" << endl;
    }
}

void CU::store(int idxReg, int idxMem, Register& reg, Memory& memory) {
    if (isValidRegisterIndex(idxReg)) {
        int value = reg.getCell(idxReg);
        ALU alu;
        string hexValue = alu.decToHex(value);
        memory.setCell(idxMem, hexValue);

        string asciiValue;
        for (size_t i = 0; i < hexValue.length(); i += 2) {
            string byte = hexValue.substr(i, 2);
            char asciiChar = static_cast<char> (stoi(byte, nullptr, 16));
            asciiValue += asciiChar;
        }

        cout << "Memory hexa output of index " << idxMem << " will be " << hexValue << endl;
        cout << "Memory ASCII output of index " << idxMem << " will be " << asciiValue << endl;

    }
}



void CU::move(int idxReg1, int idxReg2, Register& reg) {
    if (isValidRegisterIndex(idxReg1) && isValidRegisterIndex(idxReg2)) {
        int value = reg.getCell(idxReg1);
        reg.setCell(idxReg2, value);
    }
    else {
        cerr << "Invalid register index in move operation!" << endl;
    }
}


void CU::jump(int idxReg, int idxMem, Register& reg, int& PC) {

        if (idxReg == 0 || isValidRegisterIndex(idxReg) && reg.getCell(idxReg) == reg.getCell(0) ) {
            PC = idxMem;
        }
        else {
            PC += 2;
        }
    }


void CU::halt(Machine& machine) {
    cout << "Execution halted." << endl;
    machine.setHalted(true);
}


//CPU
void CPU::fetch(Memory& memory) {
    string byte1 = memory.getCell(programCounter);
    string byte2 = memory.getCell(programCounter + 1);
    instructionRegister = byte1 + byte2;
}

vector<int> CPU::decode() {
    vector<int> decodedValues;

    if (instructionRegister.length() != 4) {
        cout << "Invalid instruction length: " << instructionRegister << endl;
        return decodedValues;
    }

    int opcode = stoi(instructionRegister.substr(0, 1), nullptr, 16);
    decodedValues.push_back(opcode);

    if(opcode ==4) {
        int regIdx1 = stoi(instructionRegister.substr(instructionRegister.size() - 2, 1), nullptr, 16);
        int regIdx2 = stoi(instructionRegister.substr(instructionRegister.size() - 1, 1), nullptr, 16);
        decodedValues.push_back(regIdx1);
        decodedValues.push_back(regIdx2);
    }

    else if (opcode == 5 || opcode == 6) {
        int regIdxR = stoi(instructionRegister.substr(1, 1), nullptr, 16);
        int regIdxS = stoi(instructionRegister.substr(2, 1), nullptr, 16);
        int regIdxT = stoi(instructionRegister.substr(3, 1), nullptr, 16);
        decodedValues.push_back(regIdxR);
        decodedValues.push_back(regIdxS);
        decodedValues.push_back(regIdxT);
    }

    else {
        int reg = stoi(instructionRegister.substr(1, 1), nullptr, 16);
        int address = stoi(instructionRegister.substr(2, 2), nullptr, 16);
        decodedValues.push_back(reg);
        decodedValues.push_back(address);
    }
    return decodedValues;
}

void CPU::execute(Memory& memory, const vector<int>& decodedValues, Machine& machine) {
    if (decodedValues.empty()) return;

    int opcode = decodedValues[0];
    if (opcode == 4) {
        if(decodedValues.size() >=3){
        int regIdx1 = decodedValues[1];
        int regIdx2 = decodedValues[2];
        cu.move(regIdx1, regIdx2, reg);}
    }

    else if (opcode == 5) {
        if (decodedValues.size() >= 4) {
            int regIdxR = decodedValues[1];
            int regIdxS = decodedValues[2];
            int regIdxT = decodedValues[3];
            alu.add(regIdxS, regIdxT, regIdxR, reg);
        }
    }

    else if (opcode == 6) {
        if (decodedValues.size() >= 4) {
            int regIdxR = decodedValues[1];
            int regIdxS = decodedValues[2];
            int regIdxT = decodedValues[3];
            alu.addFloat(regIdxS, regIdxT, regIdxR, reg);
        }
    }

    else {
        int regIdx = decodedValues[1];
        int address = decodedValues[2];

        switch (opcode) {
            case 1:
                cu.load(regIdx, address, reg, memory);
                break;
            case 2:
                cu.load(regIdx, address, reg);
                break;
            case 3:
                cu.store(regIdx, address, reg, memory);
                break;
            case 0xB:
                cu.jump(regIdx, address, reg, programCounter);
            case 0xC:
                cu.halt(machine);
                break;
            default:
                cout << "Unknown opcode: " << opcode << endl;
            break;
        }
    }
    programCounter += 2;
}

void CPU::runNextStep(Memory& memory, Machine& machine) {
    fetch(memory);
    vector<int> decodedInstruction = decode();
    if (!decodedInstruction.empty()) {
        execute(memory, decodedInstruction, machine);
    }
}

int CPU::getProgramCounter() const {
    return programCounter;
}

string CPU::getInstructionRegister() const {
    return instructionRegister;
}

int CPU::getRegisterValue(int index) const {
    return reg.getCell(index);
}


//Machine
void Machine::loadProgramFile() {
    string fileName;
    cout << "Enter the path of the program file: ";
    cin >> fileName;

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file '" << fileName << "'." << endl;
        return;
    }

    string line;
    int memoryAddress = 0;
    while (getline(file, line)) {
        line = line.substr(0, 4);
        if (line.length() == 4) {
            memory.setCell(memoryAddress, line.substr(0, 2));
            memory.setCell(memoryAddress + 1, line.substr(2, 2));
            memoryAddress += 2;
        }
        else {
            cout << "Invalid instruction format: " << line << endl;
        }
    }
    file.close();
    cout << "Program loaded successfully into memory." << endl;


}

void Machine::outputState() {
    cout << endl;
    cout << "Program Counter: " << processor.getProgramCounter() << endl;
    cout << "Instruction Register: " << processor.getInstructionRegister() << endl;
    cout << "Memory" << endl;

    for (int rows = 0; rows < 16; ++rows) {
        for (int j = 0; j < 16; ++j) {
            int index = rows * 16 + j;
            cout << setw(2) << setfill('0') << hex << uppercase << index << ":";

            string value = memory.getCell(index);
            if (value.empty()) {
                cout << "00 ";
            } else {
                cout << value << " ";
            }
        }
        cout << endl;
    }


    cout << endl << "Registers" << endl;
    for (int i = 0; i < 16; ++i) {
        cout << "R" << i << ":" ;
        int value=processor.getRegisterValue(i);
        if(value!=0) {
            cout << value << "  ";

        }
        else {
            cout << "00"<< "  ";
        }
    }
    cout << endl;
}


int main() {
    Machine machine;
    bool running = true;
    bool programLoaded = false;

    while (!programLoaded) {
        machine.loadProgramFile();
        programLoaded = true;
    }

    while (running) {
        cout << endl;
        cout << "1. Run Step-by-Step" << endl;
        cout << "2. Run the whole program" << endl;
        cout << "3. Display Current State" << endl;
        cout << "4. Exit" << endl;
        cout << "Please select an option (1-4): ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                machine.runNextStep();
                break;
            case 2:
                while (!machine.isHalted()) {
                    machine.runNextStep();
                }
                machine.outputState();
                running = false;
                break;
            case 3:
                machine.outputState();
                break;
            case 4:
                running = false;
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
    return 0;
}


#include <stdio.h>
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <queue>
#include <map>

#include "Cmp.h"

using namespace std;

string regs_32_cmp[8] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
string regs_8_cmp[8] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};


void convert_binary_cmp(short *encodings)
{
    string num1 = bitset<8>(encodings[1]).to_string();
    cout << num1 << "\n";
}

int get_bits_cmp(int pos, int noOfBits, int number)
{
    return (((1 << noOfBits) - 1) & (number >> (pos - 1)));
}

int assemble_bits_cmp(int bytes, bool SIB, queue<short> &instruction)
{
    int x, offset;
    short displacement[4];

    stringstream stream, comb_no;

    for (int count = 0; count < bytes; count++)
    {
        displacement[count] = instruction.front();
        instruction.pop();
    }
    for (int i = 0; i < bytes; i++)
    {
        stream << setw(2) << setfill('0') << hex << displacement[3 - i];
    }

    string result(stream.str());

    comb_no << hex << result;
    comb_no >> x;

    //cout << result << "\n";
    //cout << x << "\n";
    return x;
}

string decode_displacement_with_SIB_cmp(int w, int d, int mod, int reg, int index, int scale, int base, queue<short> &instruction, map<string, int> &registers)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp;

    if (mod == 0 and base != 5)
    {
    }
    else
    {
        disp = assemble_bits_cmp(bytes, true, instruction);
    }

    if (mod == 0)
    {
        if (base == 5)
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "(,%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    return to_string(disp) + ",%" + regs_8_cmp[reg] + "\n";
                }
                else
                {
                    return to_string(disp) + "(,%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_8_cmp[reg] + "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "\n";
                }
                else
                {
                    return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "(,%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    return to_string(disp) + ",%" + regs_32_cmp[reg] + "\n";
                }
                else
                {
                    return to_string(disp) + "(,%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_32_cmp[reg] + "\n";
                }
            }
        }
        else
        {
            if (w == 0 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_8_cmp[reg] + "," + "(%" + regs_32_cmp[base] + ")" + "\n";
                }
                else
                {
                    return "%" + regs_8_cmp[reg] + "," + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else if (w == 0 and d == 1)
            {
                if (index == 4)
                {
                    return "(%" + regs_32_cmp[base] + "),%" + regs_8_cmp[reg] + "\n";
                }
                else
                {
                    return "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_8_cmp[reg] + "\n";
                }
            }
            else if (w == 1 and d == 0)
            {
                if (index == 4)
                {
                    return "%" + regs_32_cmp[reg] + "," + "(%" + regs_32_cmp[base] + ")" + "\n";
                }
                else
                {
                    return "%" + regs_32_cmp[reg] + "," + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
                }
            }
            else
            {
                if (index == 4)
                {
                    return "(%" + regs_32_cmp[base] + ")" + ",%" + regs_32_cmp[reg] + "\n";
                }
                else
                {
                    return "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_32_cmp[reg] + "\n";
                }
            }
        }
        //printf("mod equals 0");
    }
    else
    {
        //printf("mod not equals 0");
        if (w == 0 and d == 0)
        {
            if (index == 4)
            {
                return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[base] + ")" + "\n";
            }
            else
            {
                return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
            }
        }
        else if (w == 0 and d == 1)
        {
            if (index == 4)
            {
                return to_string(disp) + "(%" + regs_32_cmp[base] + "),%" + regs_8_cmp[reg] + "\n";
            }
            else
            {
                return to_string(disp) + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_8_cmp[reg] + "\n";
            }
        }
        else if (w == 1 and d == 0)
        {
            if (index == 4)
            {
                return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[base] + ")" + "\n";
            }
            else
            {
                return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + ")" + "\n";
            }
        }
        else
        {
            if (index == 4)
            {
                return to_string(disp) + "(%" + regs_32_cmp[base] + ")" + ",%" + regs_32_cmp[reg] + "\n";
            }
            else
            {
                return to_string(disp) + "(%" + regs_32_cmp[base] + ",%" + regs_32_cmp[index] + "," + to_string(scale) + "),%" + regs_32_cmp[reg] + "\n";
            }
        }
    }
}

string decode_displacement_without_SIB_cmp(int w, int d, int mod, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    int disp_bytes[] = {4, 1, 4};
    int bytes = disp_bytes[mod];

    int disp = assemble_bits_cmp(bytes, false, instruction);

    if (mod == 0)
    {
        if (w == 0 and d == 0)
        {
            return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "\n";
        }
        else if (w == 0 and d == 1)
        {
            return to_string(disp) + ",%" + regs_8_cmp[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "\n";
        }
        else
        {
            return to_string(disp) + ",%" + regs_32_cmp[reg] + "\n";
        }
    }
    else
    {
        if (w == 0 and d == 0)
        {
            return "%" + regs_8_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[rm] + ") \n";
        }
        else if (w == 0 and d == 1)
        {
            return to_string(disp) + "(%" + regs_32_cmp[rm] + "),%" + regs_8_cmp[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            return "%" + regs_32_cmp[reg] + "," + to_string(disp) + "(%" + regs_32_cmp[rm] + ") \n";
        }
        else
        {
            return to_string(disp) + "(%" + regs_32_cmp[rm] + "),%" + regs_32_cmp[reg] + "\n";
        }
    }
};

string decode_SIB_cmp(int w, int d, int mod, int reg, queue<short> &instruction, map<string, int> &registers)
{
    string stringSib;

    int scale = get_bits_cmp(7, 2, instruction.front());
    int index = get_bits_cmp(4, 3, instruction.front());
    int base = get_bits_cmp(1, 3, instruction.front());

    instruction.pop();

    //printf("scale:%d \n", scale);
    //printf("index:%d \n", index);
    //printf("base:%d \n", base);

    scale = pow(2, scale);

    stringSib = decode_displacement_with_SIB_cmp(w, d, mod, reg, index, scale, base, instruction, registers);
    return stringSib;
}

string decode_mod_00_cmp(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string00;
    if (rm == 4)
    {
        string00 = decode_SIB_cmp(w, d, 0, reg, instruction, registers);
    }
    else if (rm == 5)
    {
        string00 = decode_displacement_without_SIB_cmp(w, d, 0, reg, 5, instruction, registers);
    }
    else
    {
        if (w == 0 and d == 0)
        {
            string00 = "%" + regs_8_cmp[reg] + ",(%" + regs_32_cmp[rm] + ")\n";
        }
        else if (w == 0 and d == 1)
        {
            string00 = "(%" + regs_32_cmp[rm] + "),%" + regs_8_cmp[reg] + "\n";
        }
        else if (w == 1 and d == 0)
        {
            string00 = "%" + regs_32_cmp[reg] + ",(%" + regs_32_cmp[rm] + ")\n";
        }
        else
        {
            string00 = "(%" + regs_32_cmp[rm] + "),%" + regs_32_cmp[reg] + "\n";
        }
    }

    return string00;
}

string decode_mod_01_cmp(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string11;
    if (rm == 4)
    {
        string11 = decode_SIB_cmp(w, d, 1, reg, instruction, registers);
    }
    else
    {
        string11 = decode_displacement_without_SIB_cmp(w, d, 1, reg, rm, instruction, registers);
    }
    return string11;
}

string decode_mod_10_cmp(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    string string10;
    if (rm == 4)
    {
        string10 = decode_SIB_cmp(w, d, 2, reg, instruction, registers);
    }
    else
    {
        string10 = decode_displacement_without_SIB_cmp(w, d, 2, reg, rm, instruction, registers);
    }
    return string10;
}

string decode_mod_11_cmp(int w, int d, int reg, int rm, queue<short> &instruction, map<string, int> &registers)
{
    if (w == 0 and d == 0)
    {
        printf("w:0 and d:0 \n");

        int8_t num1 = get_bits_cmp(1, 8, registers[regs_32_cmp[reg]]);
        int8_t num2 = get_bits_cmp(1, 8, registers[regs_32_cmp[rm]]);



       
        cout << dec << num1<< endl;
       


        int8_t num3 = num1 & num2;
        // uint8_t num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32_cmp[rm]] = ((registers[regs_32_cmp[rm]]) & 0xffffff00) | (num3 & 0x000000ff);

        cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";


        //set or unset overflow flag
        // if (num1 > 0 and num2 > 0 and num3 < 0)
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        // }
        // else if (num1 < 0 and num2 < 0 and num3 > 0)
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        // }
        // else
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
        // }

        //set or unset carry flag
        // if(num4<unsigned(num1)){
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        // }else{
        //     registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        // }

        return "%" + regs_8_cmp[reg] + ",%" + regs_8_cmp[rm] + "\n";
    }
    else if (w == 0 and d == 1)
    {
        printf("w:0 and d:1 \n");
        int8_t num1 = get_bits_cmp(1, 8, registers[regs_32_cmp[reg]]);
        int8_t num2 = get_bits_cmp(1, 8, registers[regs_32_cmp[rm]]);

        
        cout << dec << num1<< endl;
        

        int8_t num3 = num1 & num2;
        // uint8_t num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32_cmp[reg]] = ((registers[regs_32_cmp[reg]]) & 0xffffff00) | (num3 & 0x000000ff);

        cout << "num1: " << dec << signed(num1) << ", num2: " << dec << signed(num2) << ", num3: " << dec << signed(num3) << "\n";

        //set or unset overflow flag
        // if (num1 > 0 and num2 > 0 and num3 < 0)
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        // }
        // else if (num1 < 0 and num2 < 0 and num3 > 0)
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        // }
        // else
        // {
        //     registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
        // }

        //set or unset carry flag
        // if(num4<unsigned(num1)){
        //     registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        // }else{
        //     registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        // }
        return "%" + regs_8_cmp[rm] + ",%" + regs_8_cmp[reg] + "\n";
    }
    else if (w == 1 and d == 0)
    {
        
        int num1 = registers[regs_32_cmp[reg]];
        int num2 = registers[regs_32_cmp[rm]];
        int num3 = num1 & num2;
        unsigned int num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32_cmp[rm]] = num3;

     
        cout << dec << num1<< endl;
      

        // cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 << ", num4: " << num4 <<"\n";

        cout << "num1: " << num1 << ", num2: " << num2 << ", num3: " << num3 <<"\n";

        //set or unset overflow flag
        if (num1 > 0 and num2 > 0 and num3 < 0)
        {
            registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        }
        else if (num1 < 0 and num2 < 0 and num3 > 0)
        {
            registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        }
        else
        {
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
        }

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }
        return "%" + regs_32_cmp[reg] + ",%" + regs_32_cmp[rm] + "\n";
    }
    else
    {
     
        int num1 = registers[regs_32_cmp[reg]];
        int num2 = registers[regs_32_cmp[rm]];
        int num3 = num1 + num2;
        unsigned int num4 = unsigned(num1)+unsigned(num2);
        registers[regs_32_cmp[reg]] = num3;

        cout << "num1: " << dec << num1 << ", num2: " << dec << num2 << ", num3: " << dec << num3 << "\n";

        //set or unset overflow flag
        if (num1 > 0 and num2 > 0 and num3 < 0)
        {
            registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        }
        else if (num1 < 0 and num2 < 0 and num3 > 0)
        {
            registers["EFLAGS"] = registers["EFLAGS"] | 0x800;
        }
        else
        {
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffff7ff;
        }

        //set or unset carry flag
        if(num4<unsigned(num1)){
            registers["EFLAGS"] = registers["EFLAGS"] | 0x1;
        }else{
            registers["EFLAGS"] = registers["EFLAGS"] & 0xfffffffe;
        }
        return "%" + regs_32_cmp[rm] + ",%" + regs_32_cmp[reg] + "\n";
    }
}


string Cmp::decode_Cmp(queue<short> &instruction, map<string, int> &registers) {

    bool d = get_bits_cmp(2, 1, instruction.front());
    bool w = get_bits_cmp(1, 1, instruction.front());
    cout<< d <<endl;
    instruction.pop();

    int mod = instruction.front() >> 6;
    int reg = get_bits_cmp(4, 3, instruction.front());
    int rm = get_bits_cmp(1, 3, instruction.front());

    // if (!instruction.empty()){
    //     instruction.pop();
    // }

     instruction.pop();



    string decoded_bytes;

    if (mod == 0)
    {
        decoded_bytes = decode_mod_00_cmp(w, d, reg, rm, instruction, registers);
    }
    else if (mod == 1)
    {
        decoded_bytes = decode_mod_01_cmp(w, d, reg, rm, instruction, registers);
    }
    else if (mod == 2)
    {
        decoded_bytes = decode_mod_10_cmp(w, d, reg, rm, instruction, registers);
    }
    else
    {
        decoded_bytes = decode_mod_11_cmp(w, d, reg, rm, instruction, registers);
    }

    cout << "CMP " << decoded_bytes;

    cout << "EAX: " << hex << registers["EAX"] << "\n";
    cout << "ECX: " << hex << registers["ECX"] << "\n";
    cout << "EFLAGS: " << hex << registers["EFLAGS"] << "\n";

    return "Adder instantiated and done";
}
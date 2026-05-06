#include <iostream>
#include <fstream>
#include <string>
#include "Encoder.hpp"
#include <list>
#include <cstring>

enum inputTypes
{
    FILES
};

void test_binary_convert(Encoder encode, int bitWidth);

void test_binary_2C_convert(Encoder encode, int bitWidth);

void print_instr_map(Encoder encode);

int main(int argc, char**argv)
{
    std::list<std::string> assemblyFiles;

    inputTypes inputMode;

    if (argc > 2)
    {
        for (int i = 1; i < argc; i++)
        {

            if (argv[i][0] == '-')
            {
                if (strcmp(argv[i],"-files") == 0)
                {
                    inputMode = inputTypes::FILES;
                }
            }
            else
            {
                switch (inputMode)
                {
                    case inputTypes::FILES:
                        assemblyFiles.push_back(argv[i]);
                        break;
                }
            }

        }
    }

    Encoder encode("MIPS32_Encoding.csv");
    std::cout << "Encoder Initialize Complete" << std::endl;

    // print_instr_map(encode);

    // test_binary_convert(encode, 5);
    
    // test_binary_2C_convert(encode, 5);

    if (assemblyFiles.size() > 0)
    {
        std::cout << "processing files" << std::endl;
        encode.process_assembly_files(assemblyFiles);
    }
}

void test_binary_convert(Encoder encode, int bitWidth)
{
    for (int i = -31; i <= -26; i ++)
    {
        std::string res = encode.int_to_binary(i,bitWidth);
        std::cout << "Original number: " << i << ", binary: " << res << std::endl;
    }

    for (int i = 26; i <= 31; i++)
    {
        std::string res = encode.int_to_binary(i,bitWidth);
        std::cout << "Original number: " << i << ", binary: " << res << std::endl;
    }
}

void test_binary_2C_convert(Encoder encode, int bitWidth)
{
    for (int i = -15; i <= -0; i ++)
    {
        std::string res = encode.int_to_2c_binary(i,bitWidth);
        std::cout << "Original number: " << i << ", 2C: " << res << std::endl;
    }

    for (int i = 26; i <= 31; i++)
    {
        std::string res = encode.int_to_2c_binary(i,bitWidth);
        std::cout << "Original number: " << i << ", 2C: " << res << std::endl;
    }
}

void print_instr_map(Encoder encode)
{
    std::cout << "Printing Instruction Map" << std::endl;

    for (const auto& [name, instr] : encode.instMap)
    {
        std::cout << name << std::endl;
    }
}

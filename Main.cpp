#include <iostream>
#include <fstream>
#include <string>
#include "Encoder.hpp"
#include <list>

enum inputTypes
{
    FILES
};

void test_binary_convert(Encoder encode, int bitWidth);

void test_binary_2C_convert(Encoder encode, int bitWidth);

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
                if (argv[i] == ("-files"))
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

    test_binary_convert(encode, 5);
    
    test_binary_2C_convert(encode, 5);
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


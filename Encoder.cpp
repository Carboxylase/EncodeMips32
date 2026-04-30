#include "Encoder.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <cmath>

Encoder::Encoder(std::string fileName)
{
    map_instruction_set(fileName);
}

void Encoder::process_assembly_files(std::list<std::string> assemblyFiles)
{
    std::list<std::string>::iterator it;

    for (it = assemblyFiles.begin(); it != assemblyFiles.end(); it++)
    {
        convert_to_binary(*it);
    }

}

std::string Encoder::int_to_binary(int source, int bitWidth)
{
    std::string binaryString = "";

    for (int i = bitWidth - 1; i < 0; i--)
    {
        int base = pow(2,i);

        if (source >= base)
        {
            source -= base;
            binaryString += "1";
        }
        else
        {
            binaryString += '0';
        }
    }

    return binaryString;

}

std::string Encoder::int_to_2c_binary(int source, int bitWidth)
{
    std::string binaryString = int_to_binary(abs(source), bitWidth);

    bool firstOneDetected = false;

    for (int i = bitWidth - 1 ; i < 0; i--)
    {
        if (firstOneDetected)
        {
            if (binaryString[i] == '0')
            {
                binaryString = '1'; 
            }
            else
            {
                binaryString = '0';
            }  
        }

        if (binaryString[i] == '1')
        {
            firstOneDetected = true;
        }
    }

    return binaryString;

}

void Encoder::convert_to_binary(std::string fileName)
{
    std::string line;

    std::ifstream assemblyFile (fileName);

    if (assemblyFile.is_open())
    {
        while (getline (assemblyFile, line))
        {
            int currIndex = 0;
            char curr = line[currIndex];

            std::string instrName = "";
            bool instrIdentified = false;

            std::map<int,std::string> parameterList;
            int parameterPosition = 0;
            std::string parameter = "";

            while (curr != *line.end())
            {
                if (instrIdentified == false)
                {
                    if (curr == ' ')
                    {
                        instrIdentified = true;
                    }
                    else
                    {
                        instrName += curr;
                    }
                }
                else
                {
                    if (curr == ',' || curr == '(' || curr == ')')
                    {
                        parameterList[parameterPosition] = parameter;
                        parameterPosition += 1;
                        parameter = "";
                    }
                    // else if (currIndex + 1 == line.length())
                    // {
                    //     parameter += curr;
                    //     parameterList[parameterPosition] = parameter;
                    //     parameterPosition += 1;
                    //     parameter = "";
                    // }
                    else
                    {
                        parameter += curr;
                    }
                    
                }

                currIndex += 1;
                curr = line[currIndex];

            }

            if (!parameter.compare("")) // if there is a parameter as the end
            {
                parameterList[parameterPosition] = parameter;
            }

            if (Encoder::instMap.find(instrName) == Encoder::instMap.end())
            {
                std::cout << "Failed to fetched" << instrName << std::endl;
            }
            else
            {
                Encoder::instruction fetchedInst = Encoder::instMap[instrName];

                // open the binary file
                // start iterating over the segment list
                // if a segment has a name (name != '') then find the value
                // write to binary file

                std::string clearFileName = fileName.erase(fileName.length()-5,4);
                std::string binaryFileName = fileName +  "_Binary.txt";
                std::ofstream binaryFile(binaryFileName);

                std::list<Encoder::segment>::iterator it;

                for (it = fetchedInst.sequence.begin(); it != fetchedInst.sequence.end(); it++)
                {
                    if (((Encoder::segment)*it).segType == Encoder::type::CONSTANT)
                    {
                        // add to file
                        binaryFile << ((Encoder::segment)*it).bits;
                    }
                    else if (((Encoder::segment)*it).segType == Encoder::type::REG)
                    {
                        // get the mapped register value
                        std::string regName = parameterList[fetchedInst.parameters[((Encoder::segment)*it).name]];

                        // get the register number from the alphabetical reg name
                        int regNum = Encoder::registers[regName];

                        // convert decimal to binary
                        std::string regNumBits = int_to_binary(regNum, ((Encoder::segment)*it).segmentLength);

                        binaryFile << regNumBits;

                    }
                    else
                    {
                        std::string value = parameterList[fetchedInst.parameters[((Encoder::segment)*it).name]];

                        std::cout << "value: " << value << std::endl;
                        std::string valueBits = int_to_2c_binary(std::stoi(value), ((Encoder::segment)*it).segmentLength);

                        binaryFile << valueBits;
                    }
                }
            }
        }
    }
}

void Encoder::map_instruction_set(std::string fileName)
{
    std::string line;

    std::ifstream instructionFile (fileName);

    if (instructionFile.is_open())
    {
        while (getline (instructionFile, line))
        {
            map_instruction(line);
        }
    }
}

void Encoder::map_instruction(std::string line)
{
    int curr_pos = 0;
    char curr = line[curr_pos];
    char prev = curr;

    Encoder::instruction tempInst;

    std::string tempParameter = "";
    int tempParameterPosition = 0;
    std::string tempFormat = "";
    std::string nameKey = "";

    // Encoder::segment tempSegment;
    std::string tempBits = "";
    std::string tempName = "";

    int count = 0;
    
    do
    {
        switch (count)
        {
            case 0:
                if (curr == ',')
                {
                    tempInst.parameters[tempParameter] = tempParameterPosition;
                    tempParameterPosition += 1;
                    tempParameter = "";
                    count = 1;
                }
                else if (curr == ' ')
                {
                    tempInst.parameters[tempParameter] = tempParameterPosition;
                    tempParameterPosition += 1;
                }
                else
                {
                    tempParameter += curr;
                }

                break;

            case 1:
                if (curr == ',')
                {
                    std::cout << "format one: " << tempFormat << std::endl;
                    tempInst.format.push_back(std::stoi(tempFormat));
                    tempFormat = "";
                    count = 2;
                }
                else if (curr == '-')
                {
                    std::cout << "format two: " << tempFormat << std::endl;
                    tempInst.format.push_back(std::stoi(tempFormat));
                    tempFormat = "";
                }
                else
                {
                    tempFormat += curr;
                }
                break;

            case 2:
                if (curr == ',')
                {
                    count = 3;
                }
                else
                {
                    nameKey += curr;
                }
                break;

            default:
                if (curr == ',')
                {

                    Encoder::segment tempSegment;
                    tempSegment.bits = tempBits;
                    tempSegment.name = tempName;
                    tempSegment.segmentLength = tempInst.format.front();
                    tempInst.format.pop_front();
                    

                    if (tempBits != "")
                    {
                        tempSegment.segType = Encoder::type::CONSTANT;
                    }
                    else
                    {
                        if (tempName[0] == 'r') // register
                        {
                            tempSegment.segType = Encoder::type::REG;
                        }                          
                        else
                        {
                            tempSegment.segType = Encoder::type::VAL;   
                        }
                    }

                    tempInst.sequence.push_back(tempSegment);

                    tempBits = "";
                    tempName = "";
                }
                else
                {
                    if (curr == '0' || curr == '1')
                    {
                        tempBits += curr;
                    }
                    else
                    {
                        tempName += curr;
                    }
                }
                break;
        }
        
        prev = curr;
        curr_pos++;
        curr = line[curr_pos];

    } while ((curr != ',' && prev != ',') ||  curr != *line.end());

    Encoder::instMap[nameKey] = tempInst;

    for (std::list<Encoder::segment>::iterator it = tempInst.sequence.begin(); it != tempInst.sequence.end(); it ++)
    {
        std::cout << ((Encoder::segment)*it).bits;
    }
    std::cout << "\n";
    
}
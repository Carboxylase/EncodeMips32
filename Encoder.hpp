#include <string>
#include <list>
#include <map>
#include <fstream>

class Encoder
{
    private:
        std::string slicedLine[11] = {};

        enum type
        {
            REG,
            VAL,
            CONSTANT
        };

        struct segment
        {
            std::string name;
            type segType;
            int segmentLength;
            std::string bits;
        };

        struct instruction
        {
            std::map<std::string,int> parameters;
            std::list<int> format;
            std::list<segment> sequence;
        };

        std::map<std::string, int> registers = {
            {"zero",0},
            {"at", 1},
            {"v0", 2},
            {"v1", 3},
            {"a0", 4},
            {"a1", 5},
            {"a2", 6},
            {"a3", 7},
            {"t0", 8},
            {"t1", 9},
            {"t2", 10},
            {"t3", 11},
            {"t4", 12},
            {"t5", 13},
            {"t6", 14},
            {"t7", 15},
            {"s0", 16},
            {"s1", 17},
            {"s2", 18},
            {"s3", 19},
            {"s4", 20},
            {"s5", 21},
            {"s6", 22},
            {"s7", 23},
            {"t8", 24},
            {"t9", 25},
            {"k0", 26},
            {"k1", 27},
            {"gp", 28},
            {"sp", 29},
            {"fp", 30},
            {"ra", 31}
        };
    public:
        std::map<std::string, instruction> instMap;

        Encoder(std::string fileName);

        // this is the main function to use
        void process_assembly_files(std::list<std::string> assemblyFiles);

        void map_instruction(std::string line);
        void map_instruction_set(std::string fileName);
        void convert_to_binary(std::string);
        std::string int_to_binary(int source, int bitWidth);
        std::string int_to_2c_binary(int source, int bitWidth);
        
};
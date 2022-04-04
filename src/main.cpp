#include <iostream>
#include <array>
#include <fstream>
#include "exceptions.h"
#include "converter.h"

using std::cin, std::cout, std::cerr, std::endl;


int main(int argc, char** argv) {
    try {
        if (argc != 3) throw CompException("The input must have two filename: input and output!");
        std::string input_filename = std::string (argv[1]);
        std::string output_filename = std::string (argv[2]);
        std::ifstream input(input_filename);
        if (!input) {
            throw CompException("Unable to open input file!");
        }
        std::ofstream output(output_filename);
        if (!output) {
            throw CompException("Unable to open output file!");
        }

        Converter converter(input, output);
        converter.convert(input, output);



    }
    catch (CompException& exp) {
        cerr << exp.what() << endl;
        return 1;
    }
    catch (std::ios::failure& exp) {
        cerr << "Problems with input/output file (" << exp.what() << ")" << endl;
        return 1;
    }
    catch (std::bad_alloc& exp) {
        cerr << "Unable to allocate memory" << endl;
        return 1;
    }
    catch (std::exception& exp) {
        cerr << "Unforeseen problems" << endl;
        return 1;
    }
    return 0;
}


#include <iostream>
#include <array>
#include <fstream>
#include "exceptions.h"
#include "converter.h"

using std::cin, std::cout, std::cerr, std::endl;


int main(int argc, char** argv) {
    try {
        if (argc != 2) throw CompException("The input must have one filename: input!");
        std::string input_filename = std::string (argv[1]);
        std::ifstream input(input_filename);
        if (!input) {
            throw CompException("Unable to open input file!");
        }
        std::ofstream output("result/main.cpp");
        if (!output) {
            throw CompException("Unable to open output file!");
        }

        Converter converter(input, output);
        converter.convert(input, output);

    }
    catch (CompException& exp) {
        cerr << exp.get_message() << endl;
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


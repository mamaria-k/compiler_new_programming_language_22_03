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
        std::ofstream output("result/m.cpp");
        if (!output) {
           // Это сообщение может быть очень сбивающим с толку для пользователя.
           // Он не знает что за файл вывода, так как он передает только файл который нужно
           // скомпилировать. Лучшим решением здесь было бы создавать его в случае, когда
           // открыть не получилось.
            throw CompException("Unable to open output file!");
        }

        Converter converter(input, output);
        converter.convert(input, output);

        // RAII позволяет не закрывать открытые файлы вручную
        // https://stackoverflow.com/questions/748014/do-i-need-to-manually-close-an-ifstream
        input.close();
        output.close();
        system("cd result/ && make");
        system("cd result/ && make clean");

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


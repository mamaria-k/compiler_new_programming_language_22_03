#include <iostream>
#include "converter.h"
#include "exceptions.h"


static std::string delete_spaces_beg_end(const std::string& str) {
    size_t pos_beg = 0, pos_end = str.size() - 1;
    size_t c = 0;
    while (str[pos_beg] == ' ' || str[pos_beg] == '\n' || str[pos_beg] == '\t') {
        pos_beg++;
        c++;
    }
    while (str[pos_end] == ' ' || str[pos_end] == '\n' || str[pos_end] == '\t') {
        pos_end--;
        c++;
    }
    return str.substr(pos_beg, str.size() - c);
}


Converter::Converter(std::ifstream& input, std::ofstream& output) {
    input.seekg(0, std::ios::end);
    _input_len = input.tellg();
    input.seekg(0, std::ios::beg);
}

void Converter::convert(std::ifstream& input, std::ofstream& output) {
    output << "#include <iostream>\n#include <variant>\n" << "using std::cin, std::cout, std::cerr, std::endl;\n\n";
    output << "int main() {\n";
    while(input.good()) {
        char str[_input_len];
        input.getline(str, _input_len, ';');
        _cur_line++;
        if (input.good()) {
            output << "\t";
            convert_line(std::string(str), output);
            output << ";\n";
        }
    }
    output << "\treturn 0;\n" << "}\n";
}

void Converter::convert_line(const std::string& input_str, std::ofstream& output) {
//    std::cout << input_str << "\n";     // тестирование
    std::string str = delete_spaces_beg_end(input_str);
    size_t pos_as = str.find('=');
    if (str.compare(0, 5, "print") == 0) {
        output << "cout << ";
        convert_exp(str.substr(6, str.size() - 7), "", output);
        output << " << \"\\n\"";
    }
    else if (pos_as != std::string::npos) {
        std::string v = convert_left_of_as(str.substr(0, pos_as), output);
        output << " = ";
        convert_exp(str.substr(pos_as + 1, str.size() - pos_as - 1), v, output);
    }
    else
        throw CompException("Incorrect line " + std::to_string(_cur_line) + "!");
}

std::string Converter::convert_left_of_as(const std::string& input_str, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    if (str.compare(0, 3, "val") == 0 || str.compare(0, 3, "var") == 0) {
        std::string new_v = str.substr(4, str.size() - 4);
        if (_vals.count(new_v) == 1 || _vars.count(new_v) == 1)
            throw CompException("Error: variable \"" + new_v + "\" already declared!");

        if (str.compare(0, 3, "val") == 0)
            output << "const Mixed " << new_v;
        else
            output << "Mixed " << new_v;
        return new_v;
    }
    else {
        if (_vals.count(str) == 1)
            throw CompException("Error: overriding a constant \"" + str + "\"");
        output << str;
        return str;
    }
}

void Converter::convert_exp(const std::string& input_str, const std::string& v, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    size_t pos_plus = str.find('+');
    size_t pos_minus = str.find('-');
    if (str.compare(0, 5, "input") == 0) {
        output << "cin >> ";
        convert_exp(str.substr(6, str.size() - 7), "", output);
    }
//    else if (pos_plus != std::string::npos) {
//        std::string left_v = delete_spaces_beg_end(str.substr(0, pos_plus));
//        output << " + ";
//        convert_exp(str.substr(pos_as + 1, str.size() - pos_as - 1), output);
//    }
//    else if (pos_minus != std::string::npos) {
//
//    }
//    else {
//
//    }
}







#include <iostream>
#include "converter.h"
#include "exceptions.h"


// This function removes spaces from the beginning and end of a string.
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

// These two functions check if a string is int ot float.
static int str_to_int(const std::string& s) {
    size_t found = s.find_first_not_of("1234567890");
    if (found != std::string::npos || s.empty()) return 0;
    return std::stoi(s);
}

static float str_to_float(const std::string& s) {
    size_t found = s.find_first_not_of("1234567890.");
    if (found != std::string::npos || s.empty() || s[0] == '.') return 0;
    return std::stof(s);
}

//This constructor creates a converter and initializes the value of the maximum string length.
Converter::Converter(std::ifstream& input, std::ofstream& output) {
    input.seekg(0, std::ios::end);
    _input_len = input.tellg();
    input.seekg(0, std::ios::beg);
}

// Эта функция делает два разительно отличающихся действия.
// По-хорошему этот класс должен вызывать класс фарсера, который отдаст
// AST, а полученный AST должен быть отдан классу генератора, который
// бы сгенерировал код.

// This function writes the required environment to the output file
// and calls the string parser line by line.
void Converter::convert(std::ifstream& input, std::ofstream& output) {
    output << "#include <iostream>\n#include \"mixed.h\"\n\n";
    output << "using std::cin, std::cout, std::cerr, std::endl;\n\n\n";
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

// Проблема этого решения в том, что это фактически хаки,
// а не полноценный парсинг. Такое решение будет оочень сложно
// расширить. И опять же смешивается парсинг и генерация кода.
//
// Текущее решение определенно будет работать, но ценность любой
// системы в ее расширяемости, всегда нужно помнить, что систему
// нужно будет улучшать и расширять.

// This function checks for +, -, input or print in a command.
void Converter::convert_line(const std::string& input_str, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    size_t pos_as = str.find('=');
    size_t pos_in = str.find("input");
    // For print invokes the inner expression parser.
    if (str.compare(0, 5, "print") == 0) {
        output << "cout << ";
        convert_exp(str.substr(6, str.size() - 7), output);
        output << R"( << "\n")";
    }
    // Parses the left side and the right side for assignment.
    else if (pos_as != std::string::npos) {
        // If there is an input, it outputs the inner expression of the input
        // and calls the input handling function.
        // Then it continues to work with the right side of the assignment.
        if (pos_in != std::string::npos) {
            output << "cout << ";
            size_t pos_end_in = str.find(')', pos_in);
            convert_exp(str.substr(pos_in + 6, pos_end_in - pos_in - 6), output);
            output << " << \"\\n\";\n";

            convert_input(str.substr(0, pos_as), output);
            output << "\t" << v_for_input;
        }
        else
            convert_left_of_as(str.substr(0, pos_as), output);
        output << " = ";
        convert_exp(str.substr(pos_as + 1, str.size() - pos_as - 1), output);
    }
    else
        throw CompException("Incorrect line " + std::to_string(_cur_line) + "!");
}

// This function checks for the existence of a variable, if it does not exist, it creates it, then reads the input into it.
// Here C++ has problems with constants. We solve them using the field is_input_val.
void Converter::convert_input(const std::string& input_str, std::ofstream& output) {
    is_input_val = true;
    output << "\t";
    std::string str_without_dec = input_str;
    if (input_str.compare(0, 3, "val") == 0 || input_str.compare(0, 3, "var") == 0) {
        str_without_dec = convert_left_of_as(input_str, output);
        output << " = Mixed(\"\");\n";
    }
    output << "\tcin >> ";
    v_for_input = convert_left_of_as(str_without_dec, output);
    output << ";\n";
    is_input_val = false;
}

// This function writes variable declarations, checks for the existence of variables,
// and redefinitions of constants.
// The case with input is also handled separately.
std::string Converter::convert_left_of_as(const std::string& input_str, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    if (str.compare(0, 3, "val") == 0 || str.compare(0, 3, "var") == 0) {
        std::string new_v = str.substr(4, str.size() - 4);
        if (_vals.count(new_v) == 1 || _vars.count(new_v) == 1)
            throw CompException("Error: variable \"" + new_v + "\" already declared!");

        // Ввиду отсутствия лексического анализа приходиться делать такие хаки
        // это выглядит довольно плохо. Также нет большого смысла разделять
        // константы и переменные по разным спискам, лучше было бы создать
        // структуру с флагом константности.
        // Так код бы стал чище.
        //
        // Вспомогательные функции вида:
        //    bool is_defined_var(string name)
        // или
        //    bool is_defined_const(string name)
        //
        // Сделали бы проверки чище:
        // Не
        //    "if (_vals.count(str) == 1)"
        // а
        //    "if (is_defined_const(str))"
        if (str.compare(0, 3, "val") == 0 && !is_input_val) {
            output << "const Mixed " << new_v;
            _vals.emplace(new_v);
        }
        else {
            output << "Mixed " << new_v;
            _vars.emplace(new_v);
        }
        return new_v;
    }
    else {
        if (_vals.count(str) == 1)
            throw CompException("Error: overriding a constant \"" + str + "\"");
        if (_vars.count(str) != 1)
            throw CompException("Variable " + str + " not defined!");
        output << str;
        return str;
    }
}

// This function converts an expression with +, -, splitting it into parts.
// In the case when there is an input in the expression (already processed above),
// it replaces it with the variable itself.
// These are identical steps.
void Converter::convert_exp(const std::string& input_str, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    size_t pos_plus = str.find('+');
    size_t pos_minus = str.find('-');
    if (str.compare(0, 5, "input") == 0) {
        if (pos_plus != std::string::npos) {
            if (pos_minus != std::string::npos)
                str.replace(0, std::min(pos_plus, pos_minus) - 1, v_for_input);
            else
                str.replace(0, pos_plus - 1, v_for_input);
        }
        else {
            if (pos_minus != std::string::npos)
                str.replace(0, pos_minus - 1, v_for_input);
            else
                str.replace(0, str.size(), v_for_input);
        }
        convert_exp(str, output);
    }
    else if (pos_plus != std::string::npos) {
        convert_elem(str.substr(0, pos_plus), output);
        output << " + ";
        convert_exp(str.substr(pos_plus + 1, str.size() - pos_plus - 1), output);
    }
    else if (pos_minus != std::string::npos) {
        convert_elem(str.substr(0, pos_minus), output);
        output << " - ";
        convert_exp(str.substr(pos_minus + 1, str.size() - pos_minus - 1), output);
    }
    else {
        convert_elem(str, output);
    }
}

// This function converts an expression without +, -.
// There are cases when it is a string, a number and a variable name.
void Converter::convert_elem(const std::string& input_str, std::ofstream& output) {
    std::string str = delete_spaces_beg_end(input_str);
    int i = str_to_int(str);
    float f = str_to_float(str);
    if (str.size() > 1 && str[0] == '\"' && str.back() == '\"')
        output << "Mixed(" << str << ")";
    else if (i != 0 || str == "0") output << "Mixed(" << i << ")";
    else if (f != 0 || str == "0") output << "Mixed(" << f << ")";
    // И снова из-за отсутствия лексического анализа возникают проблема.
    // Например, вот такое выражение не будет обработано правильно:
    //    print(" --- a - x --- ");
    // Output:
    //    Variable " not defined!
    else if (str.find(' ') == std::string::npos) {
        if (_vars.count(str) != 1 && _vals.count(str) != 1)
            throw CompException("Variable " + str + " not defined!");
        output << str;
    }
    else {
        throw CompException("Incorrect line " + std::to_string(_cur_line));
    }
}




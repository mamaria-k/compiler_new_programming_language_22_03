#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <variant>

typedef std::variant<int, float, std::string> Mixed;


class Converter {
public:
    explicit Converter(std::ifstream& input, std::ofstream& output);

    void convert(std::ifstream& input, std::ofstream& output);
    void convert_line(const std::string& input_str, std::ofstream& output);
    std::string convert_left_of_as(const std::string& input_str, std::ofstream& output);
    void convert_exp(const std::string& input_str, const std::string& v, std::ofstream& output);

private:
    size_t _input_len;
    size_t _cur_line = 0;

    std::unordered_map<std::string, Mixed> _vals{};
    std::unordered_map<std::string, Mixed> _vars{};

};


#pragma once
#include <unordered_set>
#include <string>
#include <fstream>
#include <variant>


class Converter {
public:
    explicit Converter(std::ifstream& input, std::ofstream& output);

    void convert(std::ifstream& input, std::ofstream& output);
    void convert_line(const std::string& input_str, std::ofstream& output);
    void convert_left_of_as(const std::string& input_str, std::ofstream& output);
    void convert_exp(const std::string& input_str, std::ofstream& output);
    void convert_elem(const std::string& input_str, std::ofstream& output);

private:
    size_t _input_len;
    size_t _cur_line = 0;

    std::unordered_set<std::string> _vals{};
    std::unordered_set<std::string> _vars{};

};


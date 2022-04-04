#include "mixed.h"
#include <string>


Mixed::Mixed(const std::string& s): type(Mixed::STRING), s(s) {}

Mixed::Mixed(float f): type(Mixed::FLOAT), f(f) {}

Mixed::Mixed(int i): type(Mixed::INT), i(i) {}


static int str_to_int(std::string s) {
    size_t found = s.find_first_not_of("1234567890");
    if (found != std::string::npos || s.empty()) return 0;
    return std::stoi(s);
}

static float str_to_float(std::string s) {
    size_t found = s.find_first_not_of("1234567890.");
    if (found != std::string::npos || s.empty() || s[0] == '.') return 0;
    return std::stof(s);
}

Mixed operator+(Mixed& a, Mixed& b) {
    if (a.type == Mixed::Type_t::STRING) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a.s + b.s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a.s + std::to_string(b.i));
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a.s + std::to_string(b.f));
    }
    if (a.type == Mixed::Type_t::INT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(std::to_string(a.i) + b.s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a.i + b.i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a.i + b.f);
    }
    if (a.type == Mixed::Type_t::FLOAT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(std::to_string(a.f) + b.s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a.f + b.i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a.f + b.f);
    }
    return Mixed(0);
}

// Не было указано поведение для (String - String), поэтому я использовала свою интерпретацию.

Mixed operator-(Mixed& a, Mixed& b) {
    if (a.type == Mixed::Type_t::STRING) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a.s + b.s);
        if (b.type == Mixed::Type_t::INT) return Mixed(str_to_int(a.s) - b.i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(str_to_float(a.s) - b.f);
    }
    if (a.type == Mixed::Type_t::INT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a.i - str_to_int(b.s));
        if (b.type == Mixed::Type_t::INT) return Mixed(a.i - b.i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a.i - b.f);
    }
    if (a.type == Mixed::Type_t::FLOAT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a.f - str_to_float(b.s));
        if (b.type == Mixed::Type_t::INT) return Mixed(a.f - b.i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a.f - b.f);
    }
    return Mixed(0);
}



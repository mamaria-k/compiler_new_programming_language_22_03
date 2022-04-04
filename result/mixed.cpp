#include "mixed.h"
#include <string>
#include <utility>


Mixed::Mixed(std::string  s): type(Mixed::STRING), _s(std::move(s)) {}

Mixed::Mixed(float f): type(Mixed::FLOAT), _f(f) {}

Mixed::Mixed(int i): type(Mixed::INT), _i(i) {}


static int str_to_int(const std::string& s) {
    size_t found = s.find_first_not_of("1234567890");
    if (found != std::string::npos || s.empty()) return 0;
    return std::stoi(s);
}

static float str_to_float(std::string s) {
    size_t found = s.find_first_not_of("1234567890.");
    if (found != std::string::npos || s.empty() || s[0] == '.') return 0;
    return std::stof(s);
}

Mixed operator+(const Mixed& a, const Mixed& b) {
    if (a.type == Mixed::Type_t::STRING) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a._s + b._s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a._s + std::to_string(b._i));
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a._s + std::to_string(b._f));
    }
    if (a.type == Mixed::Type_t::INT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(std::to_string(a._i) + b._s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a._i + b._i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a._i + b._f);
    }
    if (a.type == Mixed::Type_t::FLOAT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(std::to_string(a._f) + b._s);
        if (b.type == Mixed::Type_t::INT) return Mixed(a._f + b._i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a._f + b._f);
    }
    return Mixed(0);
}

// Не было указано поведение для (String - String), поэтому я использовала свою интерпретацию.

Mixed operator-(const Mixed& a, const Mixed& b) {
    if (a.type == Mixed::Type_t::STRING) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a._s + b._s);
        if (b.type == Mixed::Type_t::INT) return Mixed(str_to_int(a._s) - b._i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(str_to_float(a._s) - b._f);
    }
    if (a.type == Mixed::Type_t::INT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a._i - str_to_int(b._s));
        if (b.type == Mixed::Type_t::INT) return Mixed(a._i - b._i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a._i - b._f);
    }
    if (a.type == Mixed::Type_t::FLOAT) {
        if (b.type == Mixed::Type_t::STRING) return Mixed(a._f - str_to_float(b._s));
        if (b.type == Mixed::Type_t::INT) return Mixed(a._f - b._i);
        if (b.type == Mixed::Type_t::FLOAT) return Mixed(a._f - b._f);
    }
    return Mixed(0);
}

std::ostream& operator<<(std::ostream& out, const Mixed& a) {
    if (a.type == Mixed::Type_t::STRING) return out << a._s;
    if (a.type == Mixed::Type_t::INT) return out << std::to_string(a._i);
    if (a.type == Mixed::Type_t::FLOAT) return out << std::to_string(a._f);
    return out;
}

std::istream& operator>>(std::istream& in, Mixed& a) {
    std::string str;
    in >> str;
    a = Mixed(str);
    return in;
}



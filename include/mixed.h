#pragma once
#include <string>


// Я знаю о существовании union и variant,
// но с ними возникали некоторые трудности,
// поэтому был выбран такой способ.

class Mixed {
public:
    enum Type_t {
        INT, FLOAT, STRING
    };
    Type_t type;

    explicit Mixed(const std::string& s);
    explicit Mixed(float f);
    explicit Mixed(int i);

    friend Mixed operator+(Mixed& a, Mixed& b);
    friend Mixed operator-(Mixed& a, Mixed& b);

private:
    int i = 0;
    float f = 0;
    std::string s{};
};

Mixed operator+(Mixed& a, Mixed& b);
Mixed operator-(Mixed& a, Mixed& b);




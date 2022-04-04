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

    explicit Mixed(std::string  s);
    explicit Mixed(float f);
    explicit Mixed(int i);

    friend Mixed operator+(const Mixed& a, const Mixed& b);
    friend Mixed operator-(const Mixed& a, const Mixed& b);
    friend std::ostream& operator<<(std::ostream& out, const Mixed& a);

private:
    int _i = 0;
    float _f = 0;
    std::string _s{};
};

Mixed operator+(const Mixed& a, const Mixed& b);
Mixed operator-(const Mixed& a, const Mixed& b);
std::ostream& operator<<(std::ostream& out, const Mixed& a);




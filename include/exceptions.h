#pragma once
#include <string>
#include <exception>


class CompException: public std::exception {
public:
    explicit CompException(std::string message);
    std::string get_message() const;

private:
    std::string _all_message;
};



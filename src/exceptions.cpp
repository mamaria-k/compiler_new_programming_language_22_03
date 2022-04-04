#include "exceptions.h"

#include <utility>


CompException::CompException(std::string message): _all_message(std::move(message)) {}

std::string CompException::get_message() const {
    return _all_message;
}



#include <iostream>

using std::cin, std::cout, std::cerr, std::endl;


int main(int argc, char** args) {
    try {

    }
    catch (std::ios::failure& exp) {
        cerr << "Problems with input/output file (" << exp.what() << ")" << endl;
        return 1;
    }
    catch (std::bad_alloc& exp) {
        cerr << "Unable to allocate memory" << endl;
        return 1;
    }
    catch (std::exception& exp) {
        cerr << "Unforeseen problems" << endl;
        return 1;
    }
    return 0;
}

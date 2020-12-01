#include <iostream>
#include <cstddef>
#include <gmpxx.h>
#include <utility>
using namespace std;

int main() {
    int test_id;
    std::cin >> test_id;
    pair<string, string> c = make_;
    mpz_class a = mpz_class(c.first);
    mpz_class b = mpz_class(c.second);
    std::cout << mpz_class(a + b).get_str() << std::endl;
}
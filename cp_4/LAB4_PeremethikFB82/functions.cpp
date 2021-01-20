#include "functions.h"
#include <math.h>
#include <fstream>
#include <boost/random.hpp>

using namespace std;


bool Miller_Rabin_test(cpp_int p){

    int k = 8;
    cpp_int d = p - 1;
    cpp_int s = 0;
    while (d % 2 == 0) {
        d = d / 2;
        s++;
    }

    int counter = 0;
    for (int counter = 0; counter < k; counter++) {
        cpp_int x = gen_random(2, p - 1);
        if (gcd(x, p) > 1) return false;

        x = pow_mod_Horner(x, d, p);

        if ((x == 1) || (x == p - 1))
            continue;

        cpp_int xr = x;
        bool is_pseudo_prime = false;
        for (cpp_int r = 1; r <= s - 1; r++ ) {

            xr = x * pow_mod_Horner(xr, 2, p);
            if (xr == p - 1)
            {
                is_pseudo_prime = true;
            }
            else if (xr == 1)
            {
                return false;
            }
        }

        if (is_pseudo_prime == false)
        {
            return false;
        }

    }

    return true;
}

bool is_prime(cpp_int p)
{
    vector<int> numbers = { 2,3,5,7,11,13,17,19,23 };
    for (size_t i = 0; i < numbers.size(); i++)
    {
        if ( p%numbers[i] == 0)
        {
            return false;
        }
    }
    return Miller_Rabin_test(p);
}

cpp_int pow_mod_Horner(cpp_int x, cpp_int a, cpp_int m) {

    cpp_int y = 1;
    vector<cpp_int> ak;

    while (a != 0)
    {
        ak.push_back(a % 2);
        a /= 2;
    }

    for (int i = ak.size() - 1; i >= 0; i--)
    {
        y = (y * y) % m;
        if (ak.at(i) == 1) y = (y * x) % m;
    }
    return y;
}

cpp_int gen_random(size_t size)
{
    using namespace boost::random;
    static mt19937 mt((static_cast<unsigned int>(std::time(0))));
    uniform_int_distribution<cpp_int> ui(cpp_int(1) << (size - 1), (cpp_int(1) << size) - 1);
    return ui(mt);
}

cpp_int gen_random(cpp_int min, cpp_int max)
{
    using namespace boost::random;
    static mt19937 mt((static_cast<unsigned int>(std::time(0))));
    uniform_int_distribution<cpp_int> ui(min, max);
    return ui(mt);
}

cpp_int gen_prime(size_t size)
{
    std::ofstream output_file("numbers.txt", std::ios::app);   
    cpp_int p = gen_random(size);
    while (is_prime(p) == false) {
        output_file << p << std::endl;
        p = gen_random(size);
    }
    output_file.close();
    return p;
}

cpp_int gcd(cpp_int a, cpp_int b, cpp_int& u, cpp_int& v)
{
    if (a == 0) {
        u = 0; v = 1;
        return b;
    }
    cpp_int u1, v1;
    cpp_int d = gcd(b % a, a, u1, v1);
    u = v1 - (b / a) * u1;
    v = u1;
    return d;
}

cpp_int find_inverse(cpp_int a, cpp_int m)
{
    cpp_int u, v;

    if (gcd(a, m, u, v) != 1) {
        return 0;
    }
    else {
        //return u;
        //Для випадку від'ємного числа
        return (u % m + m) % m;
    }
}

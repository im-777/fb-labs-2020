#include "rsa.h"
#include "functions.h"
#include <fstream>

// Розмір ключа у байтах
const size_t key_size = 256;

RSAKeys GenerateKeyPair()
{
    // Стираємо файл з числами кандидатами, що не пройшли перевірку на простоту
    std::ofstream ofs;
    ofs.open("numbers.txt", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

	cpp_int p = gen_prime(key_size);
	cpp_int q = gen_prime(key_size);

    cpp_int n = p * q;

    cpp_int fi_n = (p - 1) * (q - 1);

    cpp_int e = gen_random(2, fi_n - 1);

    while (gcd(e, fi_n) != 1)
    {
        e = gen_random(2, fi_n - 1);
    }
    cpp_int d = find_inverse(e, fi_n);

    RSAKeys keys;
    keys.n = n;
    keys.e = e;
    keys.d = d;
    keys.p = p;
    keys.q = q;
	return keys;
}

cpp_int Encrypt(const cpp_int& M, const OpenKey & keys)
{
    return pow_mod_Horner(M, keys.e, keys.n);
}

cpp_int Decrypt(const cpp_int& C, const PersonalKey & keys)
{
    return pow_mod_Horner(C, keys.d, keys.n);
}

cpp_int Sign(const cpp_int& M, const PersonalKey& keys)
{
    return pow_mod_Horner(M, keys.d, keys.n);
}

bool Verify(const cpp_int& M, const cpp_int& S, const OpenKey& keys)
{
    cpp_int M1 = pow_mod_Horner(S, keys.e, keys.n);
    if (M == M1) return true;
    return false;
}

std::pair<cpp_int, cpp_int> SendKey(const cpp_int& k, const PersonalKey & Akeys, const OpenKey& Bkeys)
{

    cpp_int k1 = Encrypt(k, Bkeys);
    cpp_int S = Sign(k, Akeys);
    cpp_int S1 = Encrypt(S, Bkeys);

    return std::make_pair(k1, S1);

}

std::pair<bool, cpp_int> ReceiveKey(const cpp_int& k1, const cpp_int& S1, const OpenKey & Akeys, const PersonalKey & Bkeys)
{

    //cpp_int k = pow_mod_Horner(k1, Bkeys.d, Bkeys.n);
    //cpp_int S = pow_mod_Horner(S1, Bkeys.d, Bkeys.n);
    cpp_int k = Decrypt(k1, Bkeys);
    cpp_int S = Decrypt(S1, Bkeys);
    bool verified = Verify(k, S, Akeys);

    return std::make_pair(verified, k);
}


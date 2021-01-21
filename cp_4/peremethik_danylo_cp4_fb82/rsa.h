#pragma once
#include <vector>
#include <utility>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

struct RSAKeys
{
    cpp_int n;
    cpp_int e;            
    cpp_int d;
    cpp_int p;
    cpp_int q;

};

struct OpenKey {
    cpp_int e;
    cpp_int n;
};

struct PersonalKey {
    cpp_int d;
    cpp_int n;
};

RSAKeys GenerateKeyPair();

cpp_int Encrypt(const cpp_int& M, const OpenKey & keys);

cpp_int Decrypt(const cpp_int& C, const PersonalKey & keys);

cpp_int Sign(const cpp_int& M, const PersonalKey& keys);

bool Verify(const cpp_int& M, const cpp_int& S, const OpenKey& keys);

std::pair<cpp_int, cpp_int> SendKey(const cpp_int& k, const PersonalKey & Akeys, const OpenKey& Bkeys);

std::pair<bool, cpp_int> ReceiveKey(const cpp_int& k1, const cpp_int& S1, const OpenKey & Akeys, const PersonalKey & Bkeys);

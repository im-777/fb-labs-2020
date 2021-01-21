#pragma once
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

// Імовірнісний тест Міллера-Рабіна
bool Miller_Rabin_test(cpp_int p);
// Тест на простоту із попередніми пробними діленнями та тестом Міллера-Рабіна 
bool is_prime(cpp_int p);
// Схема Горнера швидкого піднесення до степеня
cpp_int pow_mod_Horner(cpp_int x, cpp_int a, cpp_int m);
// Генерація випадкового числа за розміром
cpp_int gen_random(size_t size);
// Генерація випадкового числа в діапазоні
cpp_int gen_random(cpp_int min, cpp_int max);
// Генерація простого числа з роміром в бітах
cpp_int gen_prime(size_t size);
// Розширений алгоритм Евкліда
cpp_int gcd(cpp_int a, cpp_int b, cpp_int& u, cpp_int& v);
// Обчислення зворотнього числа за модулем
cpp_int find_inverse(cpp_int a, cpp_int m);
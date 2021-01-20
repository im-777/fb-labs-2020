#pragma once
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

// ���������� ���� ̳�����-�����
bool Miller_Rabin_test(cpp_int p);
// ���� �� �������� �� ���������� �������� �������� �� ������ ̳�����-����� 
bool is_prime(cpp_int p);
// ����� ������� �������� ��������� �� �������
cpp_int pow_mod_Horner(cpp_int x, cpp_int a, cpp_int m);
// ��������� ����������� ����� �� �������
cpp_int gen_random(size_t size);
// ��������� ����������� ����� � �������
cpp_int gen_random(cpp_int min, cpp_int max);
// ��������� �������� ����� � ������ � ����
cpp_int gen_prime(size_t size);
// ���������� �������� ������
cpp_int gcd(cpp_int a, cpp_int b, cpp_int& u, cpp_int& v);
// ���������� ����������� ����� �� �������
cpp_int find_inverse(cpp_int a, cpp_int m);
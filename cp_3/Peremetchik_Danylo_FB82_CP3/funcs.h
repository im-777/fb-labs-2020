#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

// ������ ������ 33 �����
const string alpha = "��������������������������������";
// ������� ������ ��� ����� � �� �, 31 �����
const string alpha2 = "�������������������������������";
// ������� ����������� ������� ����� ��� ������� � 31 �����
const int M = alpha2.length() * alpha2.length();

// �������� ������
int gcd(int a,  int b);
// �������� ������ ����������
int gcd(int a, int b, int&u, int& v);
// ���������� ���������� ����� �� �������
int find_inverse(int a, int m);

// ���������� ��������� a
vector<int> find_a(int dX, int dY, int m);
// ���������� ��������� b
int find_b(int X1, int Y1, int a, int m);

//����� ����� (a,b), ��� ����� �� ���������� ����� 
vector<pair<int, int>> find_keys(int X1, int X2, int Y1, int Y2, int m);

//������������� ������ �� ������
string decode(const string& enc_txt, int a, int b, int m);

// ���������� �������� ����
bool ru_recognizer(const string& txt);

// ����� ��� �������� ������
vector<pair<int, int>> find_unique_keys(const vector<string>& most_common_ru_bigrams, const vector<string>& most_common_enc_bigrams);

// ����� �������� ����� ������� �����
string affine_cipher_analysis(const string& enc_txt, const vector<string>& most_common_bigrams);


// ������� ���������� ����
string read_txt_file(const string& fileName);
//Գ�������� �������
void filter_txt(string& txt);

int bigramStrToNum(string bigram);
string bigramNumToStr(int bigram);

int findLetterNum(char ch);
char find_letter(int num);

// ϳ�������� ������� ��������� l-���� ����� ���� (step) 
map<string, float> lgrams_step(const string& txt, size_t step);
// ϳ�������� ������� ��������� l-���� � ��������� 
map<string, float> lgrams_cross(const string& txt, size_t n);

//�������� ���������� ��� � ����
void write_stats_to_file(const string& file_name, const map<string, float>& letter_freq);
//��������� ���������� ��� � �������
void read_stats_from_file(const string& file_name, map<string, float>& letter_freq);
//��������� �� ���� ����
bool file_exists(const string& file_name);
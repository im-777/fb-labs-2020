#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

// Повний алфавіт 33 літери
const string alpha = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
// Урізаний алфавіт без літери ё та ъ, 31 літера
const string alpha2 = "абвгдежзийклмнопрстуфхцчшщьыэюя";
// Можлива максимальна кількість біграм для алфавіту з 31 літери
const int M = alpha2.length() * alpha2.length();

// Алгоритм Евкліда
int gcd(int a,  int b);
// Алгоритм Евкліда розширений
int gcd(int a, int b, int&u, int& v);
// Обчислення оберненого числа за модулем
int find_inverse(int a, int m);

// Обчислення параметру a
vector<int> find_a(int dX, int dY, int m);
// Обчислення параметру b
int find_b(int X1, int Y1, int a, int m);

//Пошук ключа (a,b), або декіка за значеннями біграм 
vector<pair<int, int>> find_keys(int X1, int X2, int Y1, int Y2, int m);

//Розшифрування тексту за ключем
string decode(const string& enc_txt, int a, int b, int m);

// Розпізнавач російської мови
bool ru_recognizer(const string& txt);

// Пошук всіх можливих ключів
vector<pair<int, int>> find_unique_keys(const vector<string>& most_common_ru_bigrams, const vector<string>& most_common_enc_bigrams);

// Аналіз афінного шифру біграмної заміни
string affine_cipher_analysis(const string& enc_txt, const vector<string>& most_common_bigrams);


// Читання текстового файл
string read_txt_file(const string& fileName);
//Фільтрація символів
void filter_txt(string& txt);

int bigramStrToNum(string bigram);
string bigramNumToStr(int bigram);

int findLetterNum(char ch);
char find_letter(int num);

// Підрахунок частоти появлення l-грам через крок (step) 
map<string, float> lgrams_step(const string& txt, size_t step);
// Підрахунок частоти появлення l-грам з перетином 
map<string, float> lgrams_cross(const string& txt, size_t n);

//Записати статистичні дані у файл
void write_stats_to_file(const string& file_name, const map<string, float>& letter_freq);
//Прочитати статичтичні дані у словник
void read_stats_from_file(const string& file_name, map<string, float>& letter_freq);
//Перевірити чи існує файл
bool file_exists(const string& file_name);
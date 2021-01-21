#include <iostream>
#include "funcs.h"
#include <Windows.h>
#include <algorithm>
#include <iterator>

using namespace std;

int main()
{
    // Для коректної роботи з буквами алфавіту функцій (tolower)
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);


    map<string, float> bigrams_txt;
    string letter_freq_path = "letter_freq.txt";
    if (file_exists(letter_freq_path)) {
        read_stats_from_file(letter_freq_path, bigrams_txt);
    }
    else {
        string statistics_txt = read_txt_file("stats_text.txt");
        filter_txt(statistics_txt);
        bigrams_txt = lgrams_cross(statistics_txt, 2);
        write_stats_to_file(letter_freq_path, bigrams_txt);
    }
    //Переносимо дані з map у vector для можливості сортування по частоті
    vector<std::pair<float, string>> bigram_freq_list;
    for (auto& it : bigrams_txt) {
        bigram_freq_list.push_back(pair<float, string>(it.second, it.first));
    }
    //сортуємо статистичні дані за спаданням появи літери
    sort(bigram_freq_list.begin(), bigram_freq_list.end(), greater<pair<float, string>>());

    cout << "П'ять біграм для мови з найбільшою частотою появи у тексті" << endl;
    for (size_t i = 0; i < 5;i++) {
        cout << bigram_freq_list[i].second << " " << bigram_freq_list[i].first << endl;
    }
    cout << endl;

    // Без перетину
    string letter_freq_no_cross_path = "letter_freq_no_cross.txt";
    map<string, float> bigrams_no_cross_txt;
    if (file_exists(letter_freq_no_cross_path)) {
        read_stats_from_file(letter_freq_no_cross_path, bigrams_no_cross_txt);
    }
    else {
        string statistics_txt = read_txt_file("stats_text.txt");
        filter_txt(statistics_txt);
        bigrams_no_cross_txt = lgrams_step(statistics_txt, 2);
        write_stats_to_file(letter_freq_no_cross_path, bigrams_no_cross_txt);
    }
    //Переносимо дані з map у vector для можливості сортування по частоті
    vector<std::pair<float, string>> bigram_no_cross_freq_list;
    for (auto& it : bigrams_no_cross_txt) {
        bigram_no_cross_freq_list.push_back(pair<float, string>(it.second, it.first));
    }
    //сортуємо статистичні дані за спаданням появи літери
    sort(bigram_no_cross_freq_list.begin(), bigram_no_cross_freq_list.end(), greater<pair<float, string>>());

    cout << "П'ять біграм для мови, без перетину, з найбільшою частотою появи у тексті" << endl;
    for (size_t i = 0; i < 5; i++) {
        cout << bigram_no_cross_freq_list[i].second << " " << bigram_no_cross_freq_list[i].first << endl;
    }
    cout << endl;

    // Читання і фільтрація шифротексту
    string txt15 = read_txt_file("15.txt");
    filter_txt(txt15);

    // З перетином
    map<string, float> bigrams_cross_encrypted_txt;
    bigrams_cross_encrypted_txt = lgrams_cross(txt15, 2);

    vector<std::pair<float, string>> bigram_cross_encrypted_list;
    for (auto& it : bigrams_cross_encrypted_txt) {
        bigram_cross_encrypted_list.push_back(pair<float, string>(it.second, it.first));
    }
    //сортуємо статистичні дані за спаданням появи літери
    sort(bigram_cross_encrypted_list.begin(), bigram_cross_encrypted_list.end(), greater<pair<float, string>>());
    
    cout << "П'ять біграм шифротексту з найбільшою частотою появи у тексті" << endl;
    for (size_t i = 0; i < 5; i++) {
        cout << bigram_cross_encrypted_list[i].second << " " << bigram_cross_encrypted_list[i].first << endl;
    }
    cout << endl;

    // Без перетину
    map<string, float> bigrams_step_encrypted_txt;
    bigrams_step_encrypted_txt = lgrams_step(txt15, 2);

    vector<std::pair<float, string>> bigram_step_encrypted_list;
    for (auto& it : bigrams_step_encrypted_txt) {
        bigram_step_encrypted_list.push_back(pair<float, string>(it.second, it.first));
    }
    //сортуємо статистичні дані за спаданням появи літери
    sort(bigram_step_encrypted_list.begin(), bigram_step_encrypted_list.end(), greater<pair<float, string>>());

    cout << "П'ять біграм шифротексту, без перетину, з найбільшою частотою появи у тексті" << endl;
    for (size_t i = 0; i < 5; i++) {
        cout << bigram_step_encrypted_list[i].second << " " << bigram_step_encrypted_list[i].first << endl;
    }
    cout << endl;

    vector<string> lst = { "ст", "но", "то", "на", "ен" };
    string dec_res3 = affine_cipher_analysis(txt15, lst);

    return 0;
}



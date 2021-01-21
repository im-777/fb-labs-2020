#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
//Повний алфавіт 33 букви
const string alpha = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
//Урізаний алфавіт без літери ё 32 букви
const string alpha2 = "абвгдежзийклмнопрстуфхцчшщъыьэюя";
// Читання текстового файл
string read_txt_file(const string& fileName);
//Фільтрація символів
void filter_txt(string& txt);
//Зашифрувати текст шрифтом Віженера
void encrypt_txt(const string& txt, const string& key, string& result);
//Розшифрувати текст 
void decrypt_txt(const string& txt, const string& key, string& result);
//підрахувати частоту появи символу у тексті
void count_symbols(const string& txt, map<char, int>& num);
//Підрахувати індекс відповідності
double coincidence_index(const string& txt);
//Індекс відповідності для блоку
double coincidence_index(const string& txt, int first, int step);
//Підрахувати статистику співпадань символу
double stat_D(const string& txt, int r);
//Підрахувати індекси відповідності для різних періодів
map<int, double> define_length(const string& txt, int r);

//vector<double> coincidence_index(const string& text, int r);

//Статистичні дані появи символу у тексті
map<string, float> statistics(const string& txt);

//Розшифрування тексту для окремих блоків
string caesar_decrypt(const string& txt, int key_len, map<string,float> letter_freq);
//Записати статистичні дані у файл
void write_stats_to_file(const string& file_name, const map<string, float>& letter_freq);
//Прочитати статичтичні дані у словник
void read_stats_from_file(const string& file_name, map<string, float>& letter_freq);
//Перевірити чи існує файл
bool file_exists(const string& file_name);

int main()
{
    // Для коректної роботи з буквами алфавіту функцій (tolower)
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Читання зразку тексту
    string txt = read_txt_file("text.txt");
    // Фільтрація (залишається лише 32 літери алфавіту)
    filter_txt(txt);

    double ci = coincidence_index(txt);
    cout << "відкритий текст" << " " << ci << endl;

    //ключі
    vector<string> keys = {
        //размер ключа 2, 3, 4, 5
        "ас", "дом", "банк", "посол",
        //размер ключа 10-15
        "абсолютизм", "гардеробщик", "горнорабочий", "консерватория", "предохранитель", "самообразование",
        //размер ключа 16-20
        "законодательство", "самопожертвование", "последовательность", "гидроэлектростанция", "непоследовательность"
    };

    for (int i = 0; i < keys.size();i++) {
        string encrypted;
        encrypt_txt(txt, keys[i], encrypted);
        double CI = coincidence_index(encrypted);
        cout << keys[i] << " " << CI << endl;
    }

    cout << "----------" << endl;

    string var15encrypted = read_txt_file("var15encrypted.txt");
    filter_txt(var15encrypted);

    cout << "Індекс відповідності шифротексту " << coincidence_index(var15encrypted) << endl;
    cout << "Індекси відповідності для шифротексту періодів різної довжини" << endl;
    map<int, double> res = define_length(var15encrypted, 30);
    for (auto it = res.begin(); it != res.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }

    cout << "----------" << endl;
    cout << "Статистика співпадінь для шифротексту періодів різної довжини" << endl;
    for (int r = 2; r <= 30; r++) {
        cout << r << " " << stat_D(var15encrypted, r) << endl;
    }
    
    //Якщо файл існує статистичні дані для мови беремо з файлу
    //В іншому випадку обчислюємо їх
    map<string, float> letter_freq;
    string letter_freq_path = "letter_freq.txt";
    if (file_exists(letter_freq_path)) {
        read_stats_from_file(letter_freq_path, letter_freq);
    }
    else {
        string statistics_txt = read_txt_file("stats_text.txt");
        filter_txt(statistics_txt);
        letter_freq = statistics(statistics_txt);
        write_stats_to_file(letter_freq_path, letter_freq);
    }

    int key_len;
    string key;
    cout << "Введіть довжину ключа"<<endl;
    cin >> key_len;
    cout << "Можливий ключ ";
    key = caesar_decrypt(var15encrypted, key_len, letter_freq);
    cout << key << endl;
    cout << "----------" << endl;
    cout << "Розшифрований текст" << endl;
    string decrypted;
    decrypt_txt(var15encrypted, key, decrypted);
    cout << decrypted <<endl;
    cout << "----------" << endl;
    cout << "Зашифрований текст" << endl;
    cout << var15encrypted;
    return 0;
}

string read_txt_file(const string& fileName) {

    std::ifstream ifs(fileName);
    std::string text;
    if (!ifs.is_open()) {
        cout << "File could not be opened!";
    }
    string line;
    // Читаємо файл по рядкам 
    while (std::getline(ifs, line)) {
        text += line + " ";
    }
    return text;
}

void filter_txt(string& txt)
{
    
    string::iterator it = txt.begin();
    while (it != txt.end()) {

        //Виконуємо перетворення літер з великих до малих
        char ch = tolower(*it);
        *it = tolower(*it);
        //Якщо символ не є літерою з алфавіту вилучаємо його
        size_t found = alpha.find(*it);
        if (found == std::string::npos) {
            char ch = *it;
            txt.erase(it);
            continue;
        }

        //Виконуємо заміну літери ё
        if (*it == 'ё') { *it = 'е'; }
        it++;
    }

}

void encrypt_txt(const string& txt, const string& key, string& result)
{
    result.clear();

    for (int i = 0; i < txt.size();i++) {
        int j = i % key.size();

        size_t found1 = alpha2.find(txt[i]);
        if (found1 == std::string::npos) {
            result.clear();
            return;
        }

        size_t found2 = alpha2.find(key[j]);
        if (found2 == std::string::npos) {
            result.clear();
            return;
        }

        int index = (found1 + found2) % alpha2.size();
        result += alpha2[index];
    }

}

void decrypt_txt(const string& txt, const string& key, string& result)
{
    for (int i = 0; i < txt.size(); i++) {
        int j = i % key.size();

        size_t found1 = alpha2.find(txt[i]);
        if (found1 == std::string::npos) {
            result.clear();
            return;
        }

        size_t found2 = alpha2.find(key[j]);
        if (found2 == std::string::npos) {
            result.clear();
            return;
        }

        int index = (found1 + alpha2.size() - found2) % alpha2.size();
        result += alpha2[index];
    }
}

void count_symbols(const string& text, map<char, int>& freq)
{
    freq.clear();
    for (int i = 0; i < text.size(); i++)
    {
        freq[text[i]] ++;
    }
}

double coincidence_index(const string& txt)
{
    map<char, int> num;
    count_symbols(txt, num);
    size_t n = txt.length();
    long sum = 0;
    for (map<char, int>::iterator it = num.begin(); it != num.end(); ++it)
    {
        sum += it->second * (it->second - 1);
    }
    double I = sum / ((double)(n) * (n - 1));
    return I;
}

double coincidence_index(const string& txt, int first, int step)
{
    map<char, int> num;
    int n = 0;
    for (int i = first; i < txt.size(); i+= step) {
        num[txt[i]]++;
        n++;
    }
    double sum = 0;
    for (map<char, int>::iterator it = num.begin(); it != num.end(); ++it)
    {
        sum += it->second * ((double)it->second - 1) / ((n) * ((double)n - 1));
    }
    //double I = sum / ((double)(n) * (n - 1));
    double I = sum;
    return I;

}

double stat_D(const string& txt, int r)
{
    double sum = 0;
    for (int i = 0; i < txt.size() - r; i++) {
        if (txt[i] == txt[i + r]) {
            sum++;
        }
    }
    return sum/txt.size();
}

map<int, double> define_length(const string& txt, int r)
{
    map<int, double> CI_map;
    if(r<2) return CI_map;

    for (int len = 2; len <= r; len++) {
        double sum_CI = 0;
        for (int start = 0; start < len; start++) {
            //створюємо блок
            string str;
            for (int i = start; i < txt.size(); i+= len) {
                str += txt[i];
            }
            //додаємо індекс відповідності для блоку
            sum_CI += coincidence_index(str);
            //sum_CI += coincidence_index(txt, start, len);
        }
        CI_map[len] = sum_CI / len;
    }
    return CI_map;
}
/*
vector<double> coincidence_index(const string& text, int r)
{
    size_t n = text.length();
    vector<double> resultCI;
    vector< map<char, int> > freqs;
    freqs.resize(r);
    for (int i = 0; i < r;i++) {
        resultCI.push_back(0);
    }
    

    for (int i = 0; i < text.size(); i++)
    {
        int j = i % r;
        //freqs[j][text[i]] = 1;
        freqs[j][text[i]]++;
    }
    for (int i = 0; i < freqs.size(); i++)
    {
        for (map<char, int>::iterator it = freqs[i].begin(); it != freqs[i].end(); ++it)
        {
            resultCI[i] += it->second *((double)it->second - 1);
        }
        int nn = n / r;
        if ((nn * r + i) < n) nn++;
        resultCI[i] = resultCI[i] /(nn*((double)nn-1));
    }

    return resultCI;
}
*/

map<string, float> statistics(const string& txt)
{
    map<string, float> monogram;
    string tmp;
    for (size_t i = 0; i < txt.size(); i++) {
        //tmp = txt.substr(i - 1, 1);
        tmp = txt[i];

        if (monogram.find(tmp) == monogram.end()) {
            monogram[tmp] = 1;
        }
        else {
            monogram[tmp] ++;
        }
    }

    for (map<string, float>::iterator it = monogram.begin(); it != monogram.end(); it++) {

        monogram[it->first] = it->second / txt.size();
    }

    return monogram;
}

string caesar_decrypt(const string& txt, int key_len, map<string, float> letter_freq)
{
    string key;
    //Переносимо дані з map у vector для можливості сортування по частоті
    vector<std::pair<float, string>> letter_freq_list;
    for (auto& it : letter_freq) {
        letter_freq_list.push_back(pair<float, string>(it.second, it.first));
    }
    //сортуємо статистичні дані за спаданням появи літери
    sort(letter_freq_list.begin(), letter_freq_list.end(), greater<pair<float, string>>());


    for (size_t index = 0; index < key_len; index++) {

        string str;
        for (int i = index; i < txt.size(); i += key_len) {
            str += txt[i];
        }
        //Отримуємо дані частоти появи символів у блоку
        map<string, float> block_letter_freq = statistics(str);
        //Переносимо дані з map у vector для можливості сортування по частоті
        vector<std::pair<float, string>> block_letter_freq_list;
        for (auto& it : block_letter_freq) {
            block_letter_freq_list.push_back(pair<float, string>(it.second, it.first));
        }
        //сортуємо дані частоти появи літер за спаданням
        sort(block_letter_freq_list.begin(), block_letter_freq_list.end(), greater<pair<float, string>>());

        //На основі символа, що зустрічається частіше виконуємо припущення
        //і додаємо літеру до ключа
        size_t found1 = alpha2.find(block_letter_freq_list[0].second);
        size_t found2 = alpha2.find(letter_freq_list[0].second);
        int indx = (found1 + alpha2.size() - found2) % alpha2.size();
        //cout << alpha2[indx] << endl;
        key += alpha2[indx];
    }
    return key;
}


void write_stats_to_file(const string& file_name, const map<string, float>& letter_freq)
{
    ofstream ofs(file_name);
    for (map<string, float>::const_iterator it = letter_freq.begin(); it != letter_freq.end(); it++) {

        ofs << it->first << " " << it->second<<endl;
    }

}

void read_stats_from_file(const string& file_name, map<string, float>& letter_freq)
{
    std::ifstream ifs(file_name);

    if (!ifs.is_open()) {
        cout << "File could not be opened!";
    }
    string letter;
    float freq;
    while (ifs>>letter>>freq) {
        letter_freq.insert(pair<string, float>(letter, freq));
    }
}

bool file_exists(const string& file_name)
{
    ifstream f(file_name);
    return f.good();
}



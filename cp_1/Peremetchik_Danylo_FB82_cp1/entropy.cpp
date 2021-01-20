#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h> 
#include <map>
#include <iomanip>


using namespace std;
// Повний алфавіт з пробілом
string alpha = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя ";
// Скорочений алфавіт з пробілом з розміром  = 32
string alpha2 = "абвгдежзийклмнопрстуфхцчшщыьэюя ";
// Читання файлу
string read_file(string fileName);
// Фільтрація від зайвих символів у тексті
void filter_txt(string& txt);
// Видалення пробілів з тексту
string remove_spaces(const string& txt);
// Підрахунок частоти появлення n-грам через крок (step) 
map<string, float> ngrams_step(const string & txt, size_t step);
// Підрахунок частоти появлення n-грам з перетином 
map<string, float> ngrams_cross(const string & txt, size_t n);
// Підрахунок етропії
double entropy(const map<string,float>& ngrams, int n);
// Підрахунок надлишковості
double redundancy(double entropy, size_t alpha_length);

// Друк частот появлення n-грам
void print_frequencies(const map<string,float>& ngrams, int n, int alpha_length);

// Друк частини данних про частоти появлення n-грам
void print_part(const map<string, float>& ngrams, int n, int alpha_length, int start, int length);

int main()
{
    // Відображення літер з використанням кодової сторінки 1251
    // замість 866
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    
    // Читаємо текст з файлу
    string text_with_spaces = read_file("text.txt");
    // Виконуємо фільтрацію тексту (залишаємо тільки літери алфавіту та пробіл)
    filter_txt(text_with_spaces);
    // Отримуємо варіант тексту без пробілів
    string text_no_spaces = remove_spaces(text_with_spaces);

    bool letters = true;
    bool spaces = false;
    bool crossing = true;
    int c = 1;

    while (c) {
        system("cls");
        cout << "Літери - 1, Біграми - 2, Вихід - 0" << endl;
        cin >> c;
        
        switch (c) {
            case 1:
                letters = true;
                break;
            case 2:
                letters = false;
                break;
            default:
                continue;
        }
        cout << "З пробілами - 1, без пробілів - 2" << endl;
        cin >> c;
        switch (c) {
        case 1:
            spaces = true;
            break;
        case 2:
            spaces = false;
            break;
        default:
            spaces = true;
        }
        if (letters == false) {
            cout << "З перетином - 1, без перетину - 2" << endl;
            cin >> c;
            switch (c) {
            case 1:
                crossing = true;
                break;
            case 2:
                crossing = false;
                break;
            default:
                crossing = true;
            }
        }
        double entr = 0;
        //double H0 = 0;
        double redund = 0;

        // Варіант літери з пробілами
        if (letters == true && spaces == true) {

            map<string, float> ngrams_with_spaces = ngrams_step(text_with_spaces, 1);
            entr = entropy(ngrams_with_spaces, 1);
            redund = redundancy(entr, alpha2.length());
            print_frequencies(ngrams_with_spaces, 1, alpha2.length());
        }
        // Варіант літери без пробілів
        else if (letters == true && spaces == false) {

            map<string, float> ngrams_no_spaces = ngrams_step(text_no_spaces, 1);
            entr = entropy(ngrams_no_spaces, 1);
            redund = redundancy(entr, alpha2.length() - 1);
            print_frequencies(ngrams_no_spaces, 1, alpha2.length() - 1);
        }
        //Варіант біграми з перетином та з пробілами
        else if (letters == false && spaces == true && crossing == true) {

            map<string, float> ngrams_with_spaces = ngrams_cross(text_with_spaces, 2);
            entr = entropy(ngrams_with_spaces, 2);
            redund = redundancy(entr, alpha2.length());
            print_frequencies(ngrams_with_spaces, 2, alpha2.length());
            
        }
        //Варіант біграми з перетином та без пробілів
        else if (letters == false && spaces == false && crossing == true) {

            map<string, float> ngrams_no_spaces = ngrams_cross(text_no_spaces, 2);
            entr = entropy(ngrams_no_spaces, 2);
            redund = redundancy(entr, alpha2.length() - 1);
            print_frequencies(ngrams_no_spaces, 2, alpha2.length() - 1);

        }
        //Варіант біграми з кроком 2 та з пробілами
        else if (letters == false && spaces == true && crossing == false) {

            map<string, float> ngrams_with_spaces = ngrams_step(text_with_spaces, 2);
            entr = entropy(ngrams_with_spaces, 2);
            redund = redundancy(entr, alpha2.length());
            print_frequencies(ngrams_with_spaces, 2, alpha2.length());

        }
        //Варіант біграми з кроком 2 та без пробілів
        else if (letters == false && spaces == false && crossing == false) {

            map<string, float> ngrams_no_spaces = ngrams_step(text_no_spaces, 2);
            entr = entropy(ngrams_no_spaces, 2);
            redund = redundancy(entr, alpha2.length() - 1);
            print_frequencies(ngrams_no_spaces, 2, alpha2.length() - 1);
        }

        cout << "Ентропія = " << entr << endl;
        cout << "Надлишковість = " << redund << endl; 

        system("pause");
    }

    return 0;
}

string read_file(string fileName) {

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
        *it = tolower(*it);
        //Якщо символ не є літерою з алфавіту або пробілом вилучаємо його
        size_t found = alpha.find(*it);
        if (found == std::string::npos) {
            txt.erase(it);
            continue;
        }
        //Виконуємо заміну літер ъ та ё
        if (*it == 'ъ') { *it = 'ь'; }
        if (*it == 'ё') { *it = 'е'; }
        it++;
    }

}

string remove_spaces(const string& txt)
{
    string txt_no_spaces;
    for (size_t i = 0; i < txt.length(); i++) {
        if (!isspace(txt[i])) {
            txt_no_spaces += txt[i];
        }
    }
    return txt_no_spaces;
}

map<string, float> ngrams_step(const string & txt, size_t step)
{
    map<string, float> ngrams;
    if (txt.length() <= step) {
        ngrams[txt] = 1;
        return ngrams;
    }
    int counter = 0;
    string tmp;
    for (size_t i = step; i < txt.length(); i+=step) {
        tmp = txt.substr(i - step, step);

        if (ngrams.find(tmp) == ngrams.end()) {
            ngrams[tmp] = 1;
        }
        else {
            ngrams[tmp] ++;
        }
        counter++;
    }

    for (map<string, float>::iterator it = ngrams.begin(); it != ngrams.end(); it++) {

        //ngrams[it->first] = it->second / txt.length();
        ngrams[it->first] = it->second / counter;
    }

    return ngrams;
}

map<string, float> ngrams_cross(const string & txt, size_t n)
{
    map<string, float> ngrams;

    if (txt.length() <= n) {
        ngrams[txt] = 1;
        return ngrams;
    }

    string tmp;
    int counter = 0;
    for (size_t i = n; i < txt.length(); i++) {
        tmp = txt.substr(i - n, n);
        ngrams[tmp] ++;
        counter++;
    }

    for (map<string, float>::iterator it = ngrams.begin(); it != ngrams.end(); it++) {

        //ngrams[it->first] = it->second / txt.length();
        ngrams[it->first] = it->second / counter;
    }

    return ngrams;
}

double entropy(const map<string,float>& ngrams, int n) {

    float entr = 0;

    for (map<string, float>::const_iterator it = ngrams.begin(); it != ngrams.end(); it++) {
        entr += it->second * log2(it->second);
    }
    return -entr/n;
}

double redundancy(double entr, size_t alpha_length)
{

    double H0 = log2(alpha_length);

    return 1 - entr / H0;

}

void print_frequencies(const map<string,float>& ngrams, int n, int alpha_length)
{
    if (n <= 0 || n > 2 || alpha2.size() < alpha_length) return;

    cout.setf(ios::fixed);
    //Друк літер
    if (n == 1) {


        for (int i = 0; i < alpha_length; i++) {
            string value1;
            value1 = alpha2[i];
            map<string, float>::const_iterator it = ngrams.find(value1);
            float value2 = 0.0;
            if (it != ngrams.end()) {
                //value2 = ngrams.at(value1);
                value2 = round(ngrams.at(value1) * 100000000) / 100000000;
            }
            cout << value1 << " " << setw(10) << value2 << endl;
        }
        
    }
    //Друк біграм
    else {
        int columns = 8;
        for (int i = 0; i < alpha_length;i+= columns) {

            print_part(ngrams, n, alpha_length, i, columns);
            cout << "-------------------------------\n";
        }
    }
}

void print_part(const map<string, float>& ngrams, int n, int alpha_length, int start, int length)
{
    for (int i = 0; i < alpha_length; i++) {
        for (int j = start; j < (start+length); j++) {
            string value1;
            value1.push_back(alpha2[i]);
            value1.push_back(alpha2[j]);

            map<string, float>::const_iterator it = ngrams.find(value1);
            float value2 = 0.0;
            if (it != ngrams.end()) {
                value2 = round(ngrams.at(value1) * 100000000) / 100000000;
            }
            cout << value1 << " " << setw(10) << value2 << " ";
        }
        cout << endl;
    }
}

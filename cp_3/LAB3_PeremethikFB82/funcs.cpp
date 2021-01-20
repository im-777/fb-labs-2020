#include "funcs.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//Алфавіт з 31 літери для прискорення пошуку індексу
map<char, int> alphaMap;

int gcd(int a, int b)
{
	int r0 = a; 
	int r1 = b;
	int r2;
	while (r1) {
		r2 = r0 % r1;
		r0 = r1;
		r1 = r2;
	}
	return r0;
}

int gcd(int a, int b, int& u, int& v)
{
	if (a == 0) {
		u = 0; v = 1;
		return b;
	}
	int u1, v1;
	int d = gcd(b % a, a, u1, v1);
	u = v1 - (b / a) * u1;
	v = u1;
	return d;

}

int find_inverse(int a, int m)
{
	int u, v;
	
	if (gcd(a, m, u, v) != 1){
		return 0;
	}
	else {
		//return u;
		//Для випадку від'ємного числа
		return (u % m + m) % m;
	}
		
}

vector<int> find_a(int dX, int dY, int m)
{
	vector<int> result;
	int d = gcd(dX, m);
	int res = 0;
	if (d == 1) {
		int res = (find_inverse(dX, m) * dY) % m;
		res = (res % m + m) % m;
		result.push_back(res);
	}
	else {
		//cout << "more then 1 solution" << endl << endl;
		if ((dY % d) == 0) {
			res = (dY / d * find_inverse(dX / d, m / d)) % (m / d);
			for (int i = 0; i < d; i++) {
				result.push_back(res + i * (m / d));
			}
		}
	}
	return result;
}

int find_b(int X1, int Y1, int a, int m)
{
	int b = ((Y1 - a * X1) % m);
	if (b < 0) b += m;
	return b;
}

vector<pair<int, int>> find_keys(int X1, int X2, int Y1, int Y2, int m)
{
	int x = X1 - X2;
	int y = Y1 - Y2;
	vector<int> a_sol = find_a(X1 - X2, Y1 - Y2, m);
	vector<pair<int, int>> keys;
	for (size_t i = 0; i < a_sol.size();i++) {
		int b = find_b(X1, Y1, a_sol[i], m);
		keys.push_back(pair<int, int>(a_sol[i], b));
	}
	return keys;
}

string decode(const string& enc_txt, int a, int b, int m)
{
	string dec_txt;
	int opp_a = find_inverse(a, m);
	for (size_t i = 2; i < enc_txt.length(); i += 2)
	{
		string bgr = "";
		bgr.push_back(enc_txt[i - 2]);
		bgr.push_back(enc_txt[i - 1]);

		int y = bigramStrToNum(bgr);
		int index = (opp_a * (y - b)) % (31 * 31);  //a=3 b=4
		if (index < 0) index += 31 * 31;
		dec_txt += bigramNumToStr(index);

	}
	return dec_txt;
}

bool ru_recognizer(const string& txt)
{
	
	map<string, float> monograms;
	monograms = lgrams_step(txt, 1);
	if (monograms.size() != alpha2.size()) {
		cout << "size = " << monograms.size() << endl << endl;
		return false;
	}

	vector<std::pair<float, string>> monograms_sorted;
	for (auto& it : monograms) {
		monograms_sorted.push_back(pair<float, string>(it.second, it.first));
	}
	sort(monograms_sorted.begin(), monograms_sorted.end(), greater<pair<float, string>>());
	size_t s = monograms_sorted.size();

	int count = 0;
	for (size_t i = 0; i < 3; i++) {
		char x = monograms_sorted[i].second[0];
		if (x == 'о' || x == 'а' || x == 'е') count++;
	}
	
	for (size_t i = s - 3; i < s; i++) {
		char x = monograms_sorted[i].second[0];
		if (x == 'ф' || x == 'щ' || x == 'ь') count++;
	}
	if (count >= 4) {
		cout << "Розпізнавач російської мови. " << count << " з 6 літер (о, а, е ... ф, щ, ь) співпало" <<endl;
		return true;
	} 

	return false;
}

vector<pair<int, int>> find_unique_keys(const vector<string>& most_common_ru_bigrams, const vector<string>& most_common_enc_bigrams)
{
	vector<pair<int, int>> unique_keys;
	size_t s = most_common_ru_bigrams.size();
	for (size_t i = 0; i < s; i++) {
		for (size_t j = 0; j < s; j++) {
			if (i == j) continue;
			int x1 = bigramStrToNum(most_common_ru_bigrams[i]);
			int x2 = bigramStrToNum(most_common_ru_bigrams[j]);
			int y1 = bigramStrToNum(most_common_enc_bigrams[0]);
			int y2 = bigramStrToNum(most_common_enc_bigrams[1]);
			vector<pair<int, int>> keys = find_keys(x1, x2, y1, y2, M);

			for (size_t k = 0; k < keys.size(); k++) {

				if (std::find(unique_keys.begin(), unique_keys.end(), keys[k]) == unique_keys.end()) {
					unique_keys.push_back(keys[k]);
				}
				else {
					cout << "keys is not unique" << endl;
				}
			}

		}
	}
	return unique_keys;
}

string affine_cipher_analysis(const string& enc_txt, const vector<string>& most_common_bigrams)
{
	map<string, float> bigrams_encrypted_txt;
	bigrams_encrypted_txt = lgrams_step(enc_txt, 2);

	vector<std::pair<float, string>> bigram_encrypted_list;
	for (auto& it : bigrams_encrypted_txt) {
		bigram_encrypted_list.push_back(pair<float, string>(it.second, it.first));
	}

	//сортуємо статистичні дані за спаданням появи літери
	sort(bigram_encrypted_list.begin(), bigram_encrypted_list.end(), greater<pair<float, string>>());
	
	vector<string> most_common_enc_bigrams;
	most_common_enc_bigrams.push_back(bigram_encrypted_list[0].second);
	most_common_enc_bigrams.push_back(bigram_encrypted_list[1].second);

	vector<pair<int, int>> pos_keys = find_unique_keys(most_common_bigrams, most_common_enc_bigrams);
	//Список можливих ключів
	cout << "Список можливих ключів " << endl;
	for (size_t i = 0, count = 1; i < pos_keys.size();i++, count++) {
		cout << "( "<< pos_keys[i].first << ", " << pos_keys[i].second << " )\t";
		if (count == 3) {
			count = 0;
			cout << endl;
		}
	}
	cout << endl;
	for (size_t i = 0; i < pos_keys.size();i++) {

		
		string dec_res = decode(enc_txt, pos_keys[i].first, pos_keys[i].second, M);
		if (ru_recognizer(dec_res)) {
			//return dec_res;
			cout << "Знайдено ключ до тексту" <<endl;
			cout << "( " << pos_keys[i].first << ", " << pos_keys[i].second << " ) "<<endl;
			cout << "Розшифрований текст" << endl;
			cout << dec_res<< endl << endl;
		}
	}
	return string();
}

string read_txt_file(const string& fileName)
{
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
		if (*it == 'ъ') { *it = 'ь'; }
		it++;
	}
}

int bigramStrToNum(string bigram)
{
	int m = alpha2.length();
	int ch1 = findLetterNum(bigram[0]);
	int ch2 = findLetterNum(bigram[1]);
	return ch1*m + ch2;
}

string bigramNumToStr(int bigram)
{
	int m = alpha2.length();
	int ch1 = bigram / m;
	int ch2 = bigram % m;
	string res = "";
	char c1 = find_letter(ch1);
	char c2 = find_letter(ch2);
	res.push_back(c1);
	res.push_back(c2);
	//res = findLetter(ch1) + findLetter(ch2);
	return res;
}

int findLetterNum(char ch)
{
	if (alphaMap.size() == 0) {
		for (size_t i = 0; i < alpha2.length(); i++) {
			alphaMap[alpha2[i]] = i;
		}
	}

	return alphaMap[ch];
}

char find_letter(int num)
{
	return alpha2[num];
}

map<string, float> lgrams_step(const string& txt, size_t step)
{
	map<string, float> ngrams;
	if (txt.length() <= step) {
		ngrams[txt] = 1;
		return ngrams;
	}
	int counter = 0;
	string tmp;
	for (size_t i = step; i < txt.length(); i += step) {
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

		ngrams[it->first] = it->second / counter;
	}

	return ngrams;
}

map<string, float> lgrams_cross(const string& txt, size_t n)
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

		ngrams[it->first] = it->second / counter;
	}

	return ngrams;
}

void write_stats_to_file(const string& file_name, const map<string, float>& letter_freq)
{
	ofstream ofs(file_name);
	for (map<string, float>::const_iterator it = letter_freq.begin(); it != letter_freq.end(); it++) {

		ofs << it->first << " " << it->second << endl;
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
	while (ifs >> letter >> freq) {
		letter_freq.insert(pair<string, float>(letter, freq));
	}
}

bool file_exists(const string& file_name)
{
	ifstream f(file_name);
	return f.good();
}
                                                                                                                                                                                                                                       
#include <iostream>
#include <Windows.h>
#include "functions.h"
#include "rsa.h"

using namespace std;
using namespace boost::multiprecision;

void printKeys(const RSAKeys& keys);
OpenKey getOpenKey(const RSAKeys& keys);
PersonalKey getPersonalKey(const RSAKeys& keys);

int main()
{
	setlocale(LC_ALL, "");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	int option;
	cout << "Виберіть один з варіантів"<<endl;
	cout << "1. Локальний тест" << endl;
	cout << "2. Тест з використанням сайту" << endl;
	cin >> option;
	cout << endl;
	cout << std::hex;
	cout << std::uppercase;

	if (option == 1) {
		cout << "Генерація ключів для A та B >>" << endl;
		RSAKeys Akeys = GenerateKeyPair();
		RSAKeys Bkeys = GenerateKeyPair();

		while (Bkeys.n < Akeys.n)
		{
			cout << "Повторна генерація ключа для B >>" << endl;
			Bkeys = GenerateKeyPair();
		}
		cout << endl;

		OpenKey openKeyA = getOpenKey(Akeys);
		OpenKey openKeyB = getOpenKey(Bkeys);

		PersonalKey personalKeyA = getPersonalKey(Akeys);
		PersonalKey personalKeyB = getPersonalKey(Bkeys);

		cout << "A. Дані ключів >>" << endl;
		printKeys(Akeys);
		cout << endl;
		cout << "B. Дані ключів >>" << endl;
		printKeys(Bkeys);
		cout << endl;

		cpp_int message = gen_random(1, Akeys.n - 1);
		cout << "Відкрите повідомлення =" << endl << message << endl;
		cout << endl;

		cout << "Відсилка повідомлення від A до B >>" << endl;
		pair<cpp_int, cpp_int> kS = SendKey(message, personalKeyA, openKeyB);

		cout << "k1 = " << kS.first << endl;
		cout << "S1 = " << kS.second << endl;
		cout << endl;

		cout << "Отримання повідомленння для B від A >>" << endl;
		pair<bool, cpp_int> receiveKey = ReceiveKey(kS.first, kS.second, openKeyA, personalKeyB);

		std::cout << std::boolalpha;
		cout << "Співпадіння ключів = " << receiveKey.first << endl;
		cout << "Розшифроване повідомлення =" << endl << receiveKey.second << endl;
		cout << endl;
	}
	else {

		cout << "B. Вікритий ключ >>" << endl;
		cout << "Введіть e >>"<<endl;
		string str1, str2; 
		cin >> str1;
		cout << "Введіть n >>"<<endl;
		cin >> str2;
		OpenKey openKeyB;
		openKeyB.e = cpp_int{ "0x" + str1 };
		openKeyB.n = cpp_int{ "0x" + str2 };

		//cout << openKeyB.e << endl;
		//cout << openKeyB.n << endl;

		RSAKeys Akeys = GenerateKeyPair();
		
		while (openKeyB.n < Akeys.n)
		{
			cout << Akeys.n <<endl;
			cout << "Повторна генерація ключа для A >>" << endl;
			Akeys = GenerateKeyPair();
		}

		OpenKey openKeyA = getOpenKey(Akeys);
		PersonalKey personalKeyA = getPersonalKey(Akeys);

		cout << "A. Дані ключів >>" << endl;
		printKeys(Akeys);
		cout << endl;

		cpp_int message = gen_random(1, Akeys.n - 1);
		cout << "Відкрите повідомлення = " << endl << message << endl;
		cout << endl;

		cout << "Відсилка повідомлення від A до B >>" << endl;
		pair<cpp_int, cpp_int> kS = SendKey(message, personalKeyA, openKeyB);

		cout << "k1 = " << kS.first << endl;
		cout << "S1 = " << kS.second << endl;
		cout << endl;

	}

    return 0;
}

void printKeys(const RSAKeys& keys)
{
	cout << std::hex;
	cout << std::uppercase;
	cout << "n = " << keys.n << endl;
	cout << "e = " << keys.e << endl;
	cout << "d = " << keys.d << endl;
	cout << "p = " << keys.p << endl;
	cout << "q = " << keys.q << endl;
}

OpenKey getOpenKey(const RSAKeys& keys)
{
	OpenKey key;
	key.e = keys.e;
	key.n = keys.n;
	return key;
}

PersonalKey getPersonalKey(const RSAKeys& keys)
{
	PersonalKey key;
	key.d = keys.d;
	key.n = keys.n;
	return key;
}

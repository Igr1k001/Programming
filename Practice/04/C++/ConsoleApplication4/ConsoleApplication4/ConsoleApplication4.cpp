#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	double a;
	double b;
	int c;

	cout << "Введите значение для переменной a\n";
	cin >> a;
	cout << "Введите значение для переменной b\n";
	cin >> b;

	c = a;
	a = b;
	b = c;
	cout << a << endl << b << endl;

	cout << "Введите значение для переменной a\n";
	cin >> a;
	cout << "Введите значение для переменной b\n";
	cin >> b;

	swap(a, b);
	
	cout << a << endl << b;

}





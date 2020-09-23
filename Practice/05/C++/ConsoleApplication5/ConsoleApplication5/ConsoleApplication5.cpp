#include <iostream>
#include <cmath>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	double x0 , v , t;
	double a = 9.8;
	cout << "Введите значение для переменных x0: ";
	cin >> x0;
	cout << "Введите значение для переменных v: ";
	cin >>  v;
	cout << "Введите значение для переменных t: ";
	cin >>  t;
	cout << "Result:" << x0 + v * t - a * t * t / 2 << endl;






}





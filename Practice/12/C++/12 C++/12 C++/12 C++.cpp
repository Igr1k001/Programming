#include <iostream>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Введите число для расчёта факториала" << endl;

	long n;
	cin >> n;

	
	long long f = 1;

	for (int i = 2; i <= n; i++) {
		f *= i;
	}
	cout << f << endl;


}



#include <iostream>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	double a, b;
	char git;
	cout << "Введите вещественные числа через пробел для расчета выражения!" << endl;
	cin >> a >> git >> b;

	switch (git) {
		case '+':
			cout << a + b;
			break;
		case'-':
			cout << a - b;
			break;
		case '*':
			cout << a * b;
			break;
		case '/':
			if (b == 0) {
			cout << "Ошибка! Нельзя делить на 0.";
			return 0;
			}
			else {
			cout << a / b;
			}
	}
	return 0;




	}





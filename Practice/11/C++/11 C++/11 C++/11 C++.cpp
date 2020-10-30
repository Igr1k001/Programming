#include <iostream>

using namespace std;

int main() {

	setlocale(LC_ALL, "Russian");

	cout << "На первой строке введите число, которое хотите возвести в степень." << endl; 
	cout << "На второй строке введите число, в какую степень возводится первое число." << endl;

	double a;
	int b;

	cin >> a >> b;

	double result = 1;
	for (int i = 0; i < b; i++)
		result *= a;
	if (b < 0) {
		result = 1 / result;
	}
	cout << result;


}




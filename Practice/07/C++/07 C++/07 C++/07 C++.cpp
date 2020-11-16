﻿#include <iostream>
#include <math.h>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	double x1, x2, x3, y1, y2, y3, a, b, c, d, p, s;
	cout << "Введите число 1 или 2:" << endl << "1-ввод параметров треугольника через длины сторон." << endl << "2-ввод параметров через координаты вершин или другое целое число по модулю." << endl;
	
	cin >> d;

	while (d != 1 && d != 2) {			
		cout << "Ошибочный ввод!" << endl;	
		cout << "Введите число 1 или 2:" << endl << "1-ввод параметров треугольника через длины сторон." << endl << "2-ввод параметров через координаты вершин или другое целое число по модулю." << endl;
		cin >> d;
	}

	if (d == 1) {
		cin >> a;
		while (a <= 0) {
			cout << "Вы ввели отрицательное число, введите положительное значение числа!" << endl;
			cin >> a;
		}
		cin >> b;
		while (b <= 0) {
			cout << "Вы ввели отрицательное число, введите положительное значение числа!" << endl;
			cin >> b;
		}
		cin	>> c;
		while (c <= 0) {
			cout << "Вы ввели отрицательное число, введите положительное значение числа!" << endl;
			cin >> c;
		}
		if (a <= 0 || b <= 0 || c <= 0) {
			cout << "Некорректные данные, т.к. вы ввели отрицательное число";
			return 0;
		}
		if (a + b <= c || a + c <= b || b + c <= a) {
			cout << "Некорректные данные, т.к. при этих значениях треугольник не будет существовать. ";
			return 0;
		}
		p = (a + b + c) / 2;
		s = sqrt(p * (p - a) * (p - b) * (p - c));
		cout << "S = " << s << endl;

		return 0;
	}

	if (d == 2) {
		cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		s = fabs((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2;
		cout << "S = " << s << endl;
		return 0;
	}
}
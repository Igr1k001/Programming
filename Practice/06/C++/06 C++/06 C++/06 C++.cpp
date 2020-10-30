#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");


    double x1, x2, a, b, c, d;
    cout << "Введите 3 вещественных числа, каждое на новой строке:" << endl;
    cin >> a;
    cin >> b;
    cin >> c;
    
    if ((a == 0) && (b == 0)) {
        cout << "Выражение не имеет смысла!";
        return 0;
    }
    if ((a == 0) && (b == 0) && (c == 0)) {
        cout << "Выражение не имеет смысла!";
        return 0;
    }
    if ((b == 0) && (c == 0)) {
        cout << "x = 0";
        return 0;
        if (b == 0) {
            if (a > 0 && c < 0) {
                x1 = sqrt(-c / a);
                x2 = -sqrt(-c / a);
                cout << "x1 = " << x1 << endl;
                cout << "x2 = " << x2;
                return 0;
            }
            if (a < 0 && c > 0) {
                x1 = sqrt(-c / a);
                x2 = -sqrt(-c / a);
                cout << "x1 = " << x1 << endl;
                cout << "x2 = " << x2;
                return 0;
            }
            if (a > 0 && c > 0 || a < 0 && c < 0) {
                cout << "Корней нет!";
                return 0;
            }

        }

        if (c == 0) {
            cout << "x1 = 0" << endl;
            x2 = -(b / a);
            cout << "x2 = " << x2;
            return 0;
        }
    }
    if (a == 0) {
        x1 = (c / b);
        cout << "x = " << x1;
        return 0;
    }
    else {
        d = (b * b) - (4 * a * c);
        if (d < 0) {
            cout << "Нет действительных корней." << endl;
            return 0;
        }
        if (d == 0) {
            x1 = (-b + sqrt(d)) / (2 * a);
            cout << "x = " << x1 << endl;
            return 0;
        }
        if (d > 0) {
            x1 = (-b + sqrt(d)) / (2 * a);
            x2 = (-b - sqrt(d)) / (2 * a);
            cout << "x1 = " << x1 << endl;
            cout << "x2 = " << x2 << endl;
            return 0;
        }
    }
}
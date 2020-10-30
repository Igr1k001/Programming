#include <iostream>
#include <string>

using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "В первой строке введите время прихода одного человека, а на следующей строке другого человека:" << endl;

    string time1, time2;

    cin >> time1;
    cin >> time2;

    if (time1[2] != ':' || time2[2] != ':') {
        cout << "Неверный формат ввода";
        return 0;
    }

    int h1 = stoi(time1.substr(0, 2));
    int m1 = stoi(time1.substr(3, 4));

    int h2 = stoi(time2.substr(0, 2));
    int m2 = stoi(time2.substr(3, 4));

    if (0 > h1 || h1 > 23) {
        cout << "Неверно введены данные";
        return 0;
    }
    if (0 > m1 || m1 > 59) {
         cout << "Неверно введины данные";
         return 0;
    }
    if (0 > h2 || h2 > 23) {
        cout << "Неверно введины данные";
        return 0;
        }
    if (0 > m2 || m2 > 59) {
        cout << "Неверно введины данные";
        return 0;
        } 
    int a, b;
    a = (h1 * 60) + m1;
    b = (h2 * 60) + m2;

    if ((a - b) > 15) {
        cout << "Встреча не состоится";
        return 0;
    }
    if ((b - a) > 15) {
        cout << "Встреча не состоится";
        return 0;
    }
    if ((a - b) < 15 || (a - b) > 0) {
        cout << "Встреча состоится";
        return 0;
    }
    if ((b - a) < 15 || (b - a) > 0) {
        cout << "Встреча состоится";
        return 0;
    }

}





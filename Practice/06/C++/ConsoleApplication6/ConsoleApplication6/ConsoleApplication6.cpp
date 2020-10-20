#include <iostream>
#include <math.h>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    double a, b, c, d, x1, x2; 
    cout << "Введите коэффициенты a,b,c" << endl;
    cin >> a >> b >> c; 
    d = (b * b) - (4 * a * c); 
    cout << "Дискриминант =" << d << endl; 
    if (d == 0) 
    {
        x1 = (-b + sqrt(d)) / (2 * a);
        cout << "x =" << x1 << endl;
    }

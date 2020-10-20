#include <iostream>
#include <string>

using namespace std;

int main()
{ 
    setlocale(LC_ALL,"Russian") ;

    cout << "Результат выражения 2+2*2 = " + to_string(2 + 2 * 2);
}



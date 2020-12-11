#include <vector>
#include <iostream>
#include <random>

using namespace std;

template<class T>

void print(vector<T> mass) {
    for (int i = 0; i < mass.size(); i++)
    {
        cout << mass[i] << " ";
    }
    cout << endl;
}

template<class T>

bool is_sorted(vector<T> mass, bool des = true) {
    int size = mass.size();;
    if (des) {
        for (int i = 0; i < size - 1; i++)
        {
            if (mass[i] > mass[i + 1]) return false;
        }
    }
    else {
        for (int i = 0; i < size - 1; i++)
        {
            if (mass[i] < mass[i + 1]) return false;
        }
    }
    return true;
}

template<class T>

vector<int> Bozosort(vector<T> mass, bool des = true) {
    int size = mass.size();
    vector<int> result = mass;
    while (is_sorted(result, des) == false) {
        for (int i = 0; i < size; i++) {
            int k = std::rand() % size;
            int j = std::rand() % size;
            swap(result[k], result[j]);
        }
    }
    return result;
}

template<class T>

vector<T> Bozosort(vector<vector<T>> mass, bool des = true) {
    vector<T> result;
    for (vector<T> vec : mass) {
        for (int elem : vec) {
            result.push_back(elem);
        }
    }
    return Bozosort(result, des);
}

template<class T>

vector<T> Bozosort(T a1, T a2, T a3, bool des = true) {
    vector<T> result = { a1,a2,a3 };
    return Bozosort(result, des);
}

int main() {

    setlocale(LC_ALL, "rus");

    int a;
    cout << "На первой строке введите целое число." << endl << "На второй строке напишите значения для сортировки:" << endl;
    cin >> a;
    vector<int> vvod;
    vector<int> str;
    vector<vector<int>> matriza;
    for (int i = 1; i < a + 1; i++)
    {
        int num;
        cin >> num;
        str.push_back(num);
        vvod.push_back(num);
        if (i % int(sqrt(a)) == 0) {
            matriza.push_back(str);
            str.clear();
        }
    }
    print(Bozosort(vvod));
    print(Bozosort(vvod, false));
    print(Bozosort(matriza));
    print(Bozosort(matriza, false));
    print(Bozosort(vvod[0], vvod[1], vvod[2]));
    print(Bozosort(vvod[0], vvod[1], vvod[2], false));
}

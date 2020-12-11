
#include <vector>
#include <iostream>
#include <random>

using namespace std;

void print(vector<int> mass) {
    for (int i = 0; i < mass.size(); i++)
    {
        cout << mass[i] << " ";
    }
    cout << endl;
}

bool is_sorted(vector<int> mass, bool des = true) {
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

vector<int> Bozosort(vector<int> mass, bool des = true) {
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

vector<int> Bozosort(vector<vector<int>> mass, bool des = true) {
    vector<int> result;
    for (vector<int> vec : mass) {
        for (int elem : vec) {
            result.push_back(elem);
        }
    }
    return Bozosort(result, des);
}

vector<int> Bozosort(int a1, int a2, int a3, bool des = true) {
    vector<int> result = { a1,a2,a3 };
    return Bozosort(result, des);
}

int main() {
    int n;
    cin >> n;
    vector<int> vvod;
    vector<int> str;
    vector<vector<int>> matrica;
    for (int i = 1; i < n + 1; i++)
    {
        int num;
        cout << "На первой строке введите целое число." << endl << "На второй строке напишите значения для сортировки:" << endl;
        cin >> num;
        str.push_back(num);
        vvod.push_back(num);
        if (i % int(sqrt(n)) == 0) {
            matrica.push_back(str);
            str.clear();
        }
    }
    print(Bozosort(vvod));
    print(Bozosort(vvod, false));
    print(Bozosort(matrica));
    print(Bozosort(matrica, false));
    print(Bozosort(vvod[0], vvod[1], vvod[2]));
    print(Bozosort(vvod[0], vvod[1], vvod[2], false));
}

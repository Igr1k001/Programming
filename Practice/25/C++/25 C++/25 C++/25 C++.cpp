#include <vector>
#include <iostream>
#include <random>
using namespace std;
void print(vector<int> mass) {
    for (int i = 0; i < mass.size(); i++)
    {
        cout << mass[i] << " ";
    }
    cout << "\n";
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
int main() {
    int n;
    cin >> n;
    vector<int> navvod;
    vector<int> stroka;
    vector<vector<int>> matrica;
    for (int i = 1; i < n + 1; i++)
    {
        int num;
        cin >> num;
        stroka.push_back(num);
        navvod.push_back(num);
        if (i % int(sqrt(n)) == 0) {
            matrica.push_back(stroka);
            stroka.clear();
        }
    }
}
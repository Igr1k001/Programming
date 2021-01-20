#include <iostream>
#include "./../../practice.cpp"

#include <ostream>
#include <vector>
#include <cstdlib> // rand

// file read
#include <fstream>

// File::pwd Linux
#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX
std::string pwd() {
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	const char* path;
	if (count != -1) {
		path = dirname(result);
	}
	return std::string(path);
}
std::string cwd = pwd();

std::ostream& operator << (std::ostream& o, const std::vector<int> vector) {
	o << vector.size() << "\t| ";
	for (auto& el : vector) {
		o << &el << " ";
	}
	return o << "\n";
}

int main() {

	std::vector<int> vector;
	std::ofstream file(cwd + "/data1.txt");

	if (file.is_open()) {
		for (int i = 0; i < 64; i++) {
			vector.push_back(rand());
			file << vector;
		}
		for (int i = 0; i < 64; i++) {
			vector.pop_back();
			file << vector;
		}

		for (int i = 0; i < 64; i++) {
			vector.push_back(rand());
			file << vector;
		}
		for (int i = 0; i < 64; i++) {
			vector.pop_back();
			file << vector;
		}
	}

	else {
		std::cout << "Error on data.txt open" << std::endl;
	}

	file.close();

	return 0;
}

/*
	Ответы:
		1. Сравнив адреса элементов вектора с одинаковыми индексами,
	я пришел к выводу, что число повторяющихся подряд одинаковых
	первых элементов растет, и со временем все следующие адреса
	повторяют предыдущий с добавлением дополнительного элемента(sizeof(int)).
	Дело в том, что изначально программа уделяет минимальное возможное
	количество памяти, для сохранения производительности и вместимости,
	однако, когда мы увеличиваем вектор, то и количество выделяемой памяти
	тоже увеличивается.
		2. Нет, так как весь массив был перемещён в новую память.
	Однако если этой памяти будет недостаточно для вектора 25 элементов
	данный массив там не разместится.
		3. В data1.txt можно увидеть, что после изначального цикла
	происходит цикл удаления элементов с конца по одному, пока весь массив
	не станет пустым, каждый шаг можно увидеть в соответствующей строке.

		4. Когда оператор принимает вектор по значению, то количество
	выделенной памяти не уменьшается, поэтому при уменьшении вектора
	адреса оставшихся элементов не меняются. Но при увеличивании вектора,
	все остается таким же.
*/
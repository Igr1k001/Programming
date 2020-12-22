#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct Student
{
    std::map<std::string, int> m_exams;
    std::string m_name;
    int m_group;

    Student(std::string name, int group,
        int math, int phys, int hist, int prog)
    {
        using std::to_string;
        if (group < 1 or group > 9)
            throw std::invalid_argument("Invalid group. Got " + to_string(group));
        if (math < 0 or math > 9 or phys < 0 or phys > 9 or hist < 0 or hist > 9
            or prog < 0 or prog > 9)
            throw std::invalid_argument("Invalid notes. Got " +
                to_string(math) + ", " + to_string(phys) + ", " + to_string(hist)
                + ", " + to_string(prog));

        m_exams = { {"math", math}, {"phys", phys}, {"hist", hist}, {"prog", prog} };
        m_name = name;
        m_group = group;
    }
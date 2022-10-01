// THIS FILE IS STUB FOR INPUT AND OUTPUT DATA
// DO NOT MODIFICATE IT - IT`S PART OF ANOTHER PROJECT

// это файл-заглушка, для демонстрации как метод будет вызван.
// не модифицируйте его - возможно это часть другого проекта

#include <iostream>
#include <fstream>

#include "task.h"


#include <random>
float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0, 1); // rage 0 - 1
    return dis(e);
}

int main1()
{
    // input
    int count = 1000;

    std::ofstream out("input2.txt");
    if (out.is_open())
    {
        out << count << std::endl;
        // Triangle t;
        for (int i = 0; i < count; i++)
        {
            out << get_random() << " " << get_random() << " ";
            out << get_random() << " " << get_random() << " ";
            out << get_random() << " " << get_random() << " ";
            out << std::endl;
        }
        out.close();
    }
    return 0;
}




int main()
{
	std::vector<Triangle> triangles;
	std::vector<int> intersections;

	// input
	int count = 0;
	std::ifstream in("input2.txt");
	if (in.is_open())
	{
		in >> count;
		Triangle t{};
		for (int i = 0; i < count; i++)
		{
			in >> t.a.x >> t.a.y;
			in >> t.b.x >> t.b.y;
			in >> t.c.x >> t.c.y;
			triangles.push_back(t);
		}
		in.close();
	}

	// invoke
	Task::checkIntersections(triangles, intersections);

	if (intersections.empty())
	{
		return -1;
	}

	// output
	std::ofstream out;
	out.open("output.txt");
	if (out.is_open())
	{
		for (int i = 0; i < count; i++)
			out << i << " " << intersections[i] << std::endl;
		out.close();
	}

	return 0;
}

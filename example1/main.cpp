#include <multi_index.hpp>
#include <time.h>
#include <stdlib.h>

struct MyData
{
	int aaa;

	friend std::ostream& operator<<(std::ostream& os, const MyData& e)
	{
		os << e.aaa;
		return os;
	}
};

using MyContainer = Container<MyData>;
using MyScore = Score<MyData>;

float myrand()
{
	return rand() / (RAND_MAX + 1.0);
}

int main()
{
	srand(time(0));

	MyContainer c;
	c.insert(MyScore{ 1, 1000, MyData{1} });
	c.insert(MyScore{ 3, 500, MyData{2} });
	c.insert(MyScore{ 4, 600, MyData{3} });
	c.insert(MyScore{ 5, 700, MyData{4} });
	c.insert(MyScore{ 0, 550, MyData{5} });
	c.insert(MyScore{ 6, 800, MyData{5} });
	c.insert(MyScore{ 7, 900, MyData{5} });
	c.insert(MyScore{ 2, 1500, MyData{6} });

	for (int i = 0; i < 10; i++)
	{
		auto item = c.get_one(3, 300, 800, myrand);
		printf("%lu %lu %lu\n", item->id, item->score, item->data.aaa);
	}

	for (int i = 0; i < 10; i++)
	{
		auto items = c.get_n(2, 3, 300, 800, myrand);
		printf("%lu %lu %lu | %lu %lu %lu\n",
			items[0]->id, items[0]->score, items[0]->data.aaa,
			items[1]->id, items[1]->score, items[1]->data.aaa);
	}

	for (int i = 0; i <= 5; ++i)
	{
		c.erase(i);
	}
	printf("c size: %d\n", c.size());
}
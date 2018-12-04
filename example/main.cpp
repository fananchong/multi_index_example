#include <multi_index.hpp>

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

int main()
{
	MyContainer c;
	c.insert(MyScore{ 1, 1000, MyData{1} });
	c.insert(MyScore{ 3, 500, MyData{2} });
	c.insert(MyScore{ 4, 600, MyData{3} });
	c.insert(MyScore{ 5, 700, MyData{4} });
	c.insert(MyScore{ 0, 550, MyData{5} });
	c.insert(MyScore{ 2, 1500, MyData{6} });

	print_out_by<id>(c); printf("\n");
	print_out_by<score>(c); printf("\n");

	auto it_upper = c.get<score>().upper_bound(500 - 1);
	auto it_lower = c.get<score>().lower_bound(600 + 1);

	for (auto it = it_upper; it != it_lower; ++it)
	{
		printf("%lu %lu %lu\n", it->id, it->score, it->data.aaa);
	}

	printf("\n");

	auto it = c.get<id>().find(3);
	if (it != c.get<id>().end())
	{
		printf("%lu %lu %lu\n", it->id, it->score, it->data.aaa);
	}

}
#include <multi_index.hpp>



int main()
{
	Container c;
	c.insert(Score{ 1, 1000 });
	c.insert(Score{ 3, 500 });
	c.insert(Score{ 4, 600 });
	c.insert(Score{ 5, 700 });
	c.insert(Score{ 0, 550 });
	c.insert(Score{ 2, 1500 });

	print_out_by<id>(c); printf("\n");
	print_out_by<score>(c); printf("\n");

	auto it_upper = c.get<score>().upper_bound(500 - 1);
	auto it_lower = c.get<score>().lower_bound(600 + 1);

	for (auto it = it_upper; it != it_lower; ++it)
	{
		printf("%lu %lu\n", it->id, it->score);
	}

	printf("\n");

	auto it = c.get<id>().find(3);
	if (it != c.get<id>().end())
	{
		printf("%lu %lu\n", it->id, it->score);
	}

}
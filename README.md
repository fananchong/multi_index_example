# multi_index_example

使用 boost::multi_index 的一个工具类。

功能如下：

容器，能按某字段（如积分）插入元素自动排序，且根据某字段（如ID）快速访问元素，且根据元素能遍历该元素为中心的某区域元素，并随机选取1个。

## header only

只需要包含 #include <multi_index.hpp>

## 依赖

boost::multi_index


## 使用例子

```c++
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
		auto item = c.get_one(3, 500, myrand);
		printf("%lu %lu %lu\n", item->id, item->score, item->data.aaa);
	}

	for (int i = 0; i < 10; i++)
	{
		auto items = c.get_n(2, 3, 500, myrand);
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
```

## 函数 get_one

```c++
const Score<T>* get_one(uint64_t id_, int64_t range, std::function<float()> random_func)
std::vector<const Score<T>*> get_n(int n, uint64_t id_, int64_t range, std::function<float()> random_func)
```

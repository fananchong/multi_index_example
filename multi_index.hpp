#pragma once

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <functional>
#include <vector>

using boost::multi_index_container;
using namespace boost::multi_index;

template<class T>
struct Score
{
	uint64_t id;
	int64_t score;
	T data;

	Score(uint64_t id_, int64_t score_, const T& data_)
		: id(id_)
		, score(score_)
		, data(data_)
	{}

	friend std::ostream& operator<<(std::ostream& os, const Score& e)
	{
		os << e.id << " " << e.score << " " << e.data << std::endl;
		return os;
	}
};

struct id {};
struct score {};

template<class T>
class Container : public multi_index_container <Score<T>, indexed_by <
	ordered_unique <tag<id>, BOOST_MULTI_INDEX_MEMBER(Score<T>, uint64_t, id)>,
	ordered_non_unique <tag<score>, BOOST_MULTI_INDEX_MEMBER(Score<T>, int64_t, score)>>
	>
{
public:
	const Score<T>* get_one(uint64_t id_, int64_t range, std::function<float()> random_func)
	{
		auto ret = get_n(1, id_, range, random_func);
		return ret.size() ? ret[0] : nullptr;
	}

	std::vector<const Score<T>*> get_n(int n, uint64_t id_, int64_t range, std::function<float()> random_func)
	{
		auto it = this->get<id>().find(id_);
		if (it == this->get<id>().end())
		{
			return std::vector<const Score<T>*>();
		}
		auto s = it->score;
		auto it_upper = this->get<score>().upper_bound(s - range);
		auto it_lower = this->get<score>().lower_bound(s + range);

		std::vector<const Score<T>*> temps;
		for (auto it = it_upper; it != it_lower; ++it)
		{
			if (it->id != id_)
			{
				temps.push_back(&(*it));
			}
		}

		std::vector<const Score<T>*> ret;
		while (temps.size() > 0 && n > 0)
		{
			int index = int(random_func() * temps.size());
			ret.push_back(temps[index]);
			if (index != int(temps.size() - 1))
			{
				temps[index] = temps[temps.size() - 1];
			}
			temps.resize(temps.size() - 1);
			--n;
		}
		return ret;
	}
};

template<typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
	const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i = get<Tag>(s);
	typedef typename MultiIndexContainer::value_type value_type;
	std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
}

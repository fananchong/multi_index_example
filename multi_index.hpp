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

	Score(uint64_t id_, int score_, const T& data_)
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
		auto it = this->get<id>().find(id_);
		if (it == this->get<id>().end())
		{
			return nullptr;
		}
		auto s = it->score;
		auto it_upper = this->get<score>().upper_bound(s - range);
		auto it_lower = this->get<score>().lower_bound(s + range);

		std::vector<const Score<T>*> temps;
		for (auto it = it_upper; it != it_lower; ++it)
		{
			temps.push_back(&(*it));
		}

		if (temps.size() > 0)
		{
			int index = int(random_func() * temps.size());
			return temps[index];
		}
		return nullptr;
	}
};

template<typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
	const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i = get<Tag>(s);
	typedef typename MultiIndexContainer::value_type value_type;
	std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
}

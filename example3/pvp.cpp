#include "pvp.hpp"
#include <stdlib.h>
#include "log.hpp"

#define CHECK_TYPE(RET)	\
if (type > (int)m_pvp_containers.size())	\
{	\
	TRACEERROR("pvp type error. type:%d", type);	\
	return RET;	\
}	\

#define CONTAINER_NUM (2)
#define MAX_ROLE_LEVEL (100)
#define PVP_ROLE_LEVEL_RANGE_LIMIT (5)
#define PVP_MATCH_TIME_LIMIT (30)

PVP::PVP()
{
}

PVP::~PVP()
{

}

int PVP::init()
{
	m_pvp_containers.resize(CONTAINER_NUM);
	for (auto& c : m_pvp_containers)
	{
		c.resize(MAX_ROLE_LEVEL);
	}
	return 0;
}

int PVP::add(uint64_t roleid, int type, const PVPINFO& info)
{
	CHECK_TYPE(-1);
	auto it = m_roles.insert({ roleid, info });
	if (it.second)
	{
		auto& c = m_pvp_containers[type - 1];
		if ((int32_t)c.size() < info.role_level)
		{
			return -1;
		}
		c[info.role_level - 1].insert({ roleid, info.score, &(it.first->second) });
		return 0;
	}
	return -1;
}

void PVP::del(uint64_t roleid, int type)
{
	CHECK_TYPE();
	auto it = m_roles.find(roleid);
	if (it != m_roles.end())
	{
		auto& c = m_pvp_containers[type - 1];
		if ((int32_t)c.size() >= it->second.role_level)
		{
			c[it->second.role_level - 1].erase(roleid);
		}
		m_roles.erase(it);
	}
}

void PVP::del(const std::vector<PVPINFO*> roles, int type)
{
	CHECK_TYPE();
	auto& c = m_pvp_containers[type - 1];
	for (auto item : roles)
	{
		auto id = item->roleid;
		if ((int32_t)c.size() >= item->role_level)
		{
			c[item->role_level - 1].erase(id);
		}
		m_roles.erase(id);
	}
}

float myrand()
{
	return rand() / (RAND_MAX + 1.0);
}

std::vector<PVPINFO*> PVP::match(uint64_t roleid, int type, time_t curtime)
{
	std::vector<PVPINFO*> ret;
	auto it_role = m_roles.find(roleid);
	if (it_role == m_roles.end())
	{
		return ret;
	}
	ret.push_back(&(it_role->second));
	CHECK_TYPE(ret);
	auto range = get_range(it_role->second, curtime);
	auto n = type * 2 - 1;
	auto& c = m_pvp_containers[type - 1];

	// 先找下本等级的
	auto score1 = it_role->second.score - range;
	auto score2 = it_role->second.score + range;
	if ((int32_t)c.size() < it_role->second.role_level)
	{
		return ret;
	}
	auto match_result = c[it_role->second.role_level - 1].get_n(n, roleid, score1, score2, myrand);
	for (auto item : match_result)
	{
		ret.push_back(item->data);
	}

	// 人数不足，则查找 (-PVP_ROLE_LEVEL_RANGE_LIMIT, PVP_ROLE_LEVEL_RANGE_LIMIT) 的玩家
	if ((int)ret.size() < 2 * type)
	{
		for (int i = 1; i < PVP_ROLE_LEVEL_RANGE_LIMIT; ++i)
		{
			auto lvl = it_role->second.role_level + i;
			if (lvl <= (int)c.size())
			{
				auto match_result = c[lvl - 1].get_n(n, roleid, score1, score2, myrand);
				for (auto item : match_result)
				{
					ret.push_back(item->data);
				}
			}
			if ((int)ret.size() >= 2 * type)
			{
				break;
			}

			lvl = it_role->second.role_level - i;
			if (lvl > 0)
			{
				auto match_result = c[lvl - 1].get_n(n, roleid, score1, score2, myrand);
				for (auto item : match_result)
				{
					ret.push_back(item->data);
				}
			}
			if ((int)ret.size() >= 2 * type)
			{
				break;
			}
		}
	}
	while ((int)ret.size() > 2 * type)
	{
		ret.pop_back();
	}
	return ret;
}

void PVP::tick(time_t t)
{
	std::unordered_map<uint64_t, int> dels;
	for (auto& item : m_roles)
	{
		auto type = item.second.type;
		auto timestamp = item.second.timestamp;
		auto roleid = item.first;
		if (dels.find(roleid) != dels.end())
		{
			continue;
		}
		auto match_result = match(roleid, type, t);
		if (int(match_result.size()) >= 2 * type)
		{
			// 匹配成功
			on_match(match_result, type);
			for (auto mritem : match_result)
			{
				dels[mritem->roleid] = 1;
			}
			del(match_result, type);
		}
		else if (t - timestamp >= PVP_MATCH_TIME_LIMIT)
		{
			// 匹配超时
			on_match(match_result, type);
			for (auto mritem : match_result)
			{
				dels[mritem->roleid] = 1;
			}
			del(match_result, type);
		}
		else
		{
			// pass
		}
	}
	for (auto& item : dels)
	{
		m_roles.erase(item.first);
	}
}

int64_t PVP::get_range(const PVPINFO& info, time_t curtime)
{
	return 10000;
}

void PVP::on_match(const std::vector<PVPINFO*> roles, int type)
{
	// TODO
}

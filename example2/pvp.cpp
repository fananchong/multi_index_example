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

PVP::PVP()
{
}

PVP::~PVP()
{

}

int PVP::init()
{
	m_pvp_containers.resize(CONTAINER_NUM);
	return 0;
}

int PVP::add(uint64_t roleid, int type, const PVPINFO& info)
{
	CHECK_TYPE(-1);
	auto it = m_roles.insert({ roleid, info });
	if (it.second)
	{
		auto& c = m_pvp_containers[type - 1];
		c.insert({ roleid, info.score, &(it.first->second) });
		return 0;
	}
	return -1;
}

void PVP::del(uint64_t roleid, int type)
{
	CHECK_TYPE();
	auto& c = m_pvp_containers[type - 1];
	c.erase(roleid);
	m_roles.erase(roleid);
}

void PVP::del(const std::vector<PVPINFO*> roles, int type)
{
	CHECK_TYPE();
	auto& c = m_pvp_containers[type - 1];
	for (auto item : roles)
	{
		auto id = item->roleid;
		c.erase(id);
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
	auto score = it_role->second.score;
	auto range = get_range(it_role->second, curtime);
	auto n = type * 2 - 1;
	auto& c = m_pvp_containers[type - 1];
	auto match_result = c.get_n(n, roleid, score - range, score + range, myrand);
	for (auto item : match_result)
	{
		ret.push_back(item->data);
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
		auto& c = m_pvp_containers[type - 1];
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
		else if (t - timestamp >= 30)
		{
			// 匹配超时，可能匹配到部分人
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

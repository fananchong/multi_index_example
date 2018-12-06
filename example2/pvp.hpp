#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include <time.h>
#include "multi_index.hpp"

struct PVPINFO
{
	int type;
	time_t timestamp;
	uint64_t roleid;
	int64_t score;

	// other data

};

using PVPContainer = Container<PVPINFO*>;
using PVPScore = Score<PVPINFO*>;

class PVP
{
public:
	PVP();
	~PVP();

	int init();

	// type£¬ 1£º1v1  2£º2v2 µÈµÈ
	int add(uint64_t roleid, int type, const PVPINFO& info);
	std::vector<PVPINFO*> match(uint64_t roleid, int type, time_t curtime);
	void del(uint64_t roleid, int type);
	void del(const std::vector<PVPINFO*> roles, int type);
	void tick(time_t t);

public:
	virtual int64_t get_range(const PVPINFO& info, time_t curtime) { return 100000; }
	virtual void on_match(const std::vector<PVPINFO*> roles, int type) {}

private:
	std::vector<PVPContainer> m_pvp_containers;
	std::unordered_map<uint64_t, PVPINFO> m_roles;
};

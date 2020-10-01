#pragma once
#include "graph.h"
#include <vector>
#include <map>

using namespace std;

class NestedList : public Graph
{
	private:
	vector<map<size_t, float> > m_data;
	vector<map<size_t, float> > m_back;
	
	public:
	NestedList(size_t peaks, bool oriented);
	void add_peak();
	void rem_peak(size_t peak);
	size_t peak_count();
	void add_arc(size_t from, size_t to, float weight);
	bool rem_arc(size_t from, size_t to);
	bool has_arc(size_t from, size_t to);
	float weight(size_t from, size_t to);
	ArcList outgoing(size_t peak);
	ArcList incoming(size_t peak);
};

class NestedListBridge : public ArcBridge
{
	private: 
	map<size_t, float>::const_iterator m_it;
	
	public:	
	NestedListBridge(map<size_t, float>::const_iterator&& it)
	{
		m_it = it;
	}
	
	void next() { ++m_it; }
	
	bool equals(ArcBridge* other)
	{
		NestedListBridge* o = dynamic_cast<NestedListBridge*>(other);
		
		if (o == nullptr)
			return false;
		
		return o->m_it == m_it;
	}
	
	Arc current()
	{
		Arc ret = { m_it->first, m_it->second };
		return ret;
	}
};

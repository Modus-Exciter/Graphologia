#include "nestedlist.h"
#include <cmath>

using namespace std;

NestedList::NestedList(size_t peaks, bool oriented) : Graph(oriented)
{
	m_data.resize(peaks);
	
	if (is_oriented())
		m_back.resize(peaks);
}

void NestedList::add_peak()
{
	m_data.push_back(map<size_t, float>());
	
	if (is_oriented())
		m_back.push_back(map<size_t, float>());
}

void NestedList::rem_peak(size_t peak)
{
	check_peak(peak);
	m_data.erase(m_data.begin() + peak);

	for (map<size_t, float>& l : m_data)
	{
		l.erase(peak);
		for (size_t i = peak + 1; i <= m_data.size(); i++)
		{			
			if (l.find(i) != l.end())
			{
				float value = l[i];
				l.erase(i);
				l[i - 1] = value;
			}
		}
	}
	
	if (!is_oriented())
		return;
	
	m_back.erase(m_back.begin() + peak);
	
	for (map<size_t, float>& l : m_back)
	{
		l.erase(peak);
		for (size_t i = peak + 1; i <= m_back.size(); i++)
		{			
			if (l.find(i) != l.end())
			{
				float value = l[i];
				l.erase(i);
				l[i - 1] = value;
			}
		}
	}
}

size_t NestedList::peak_count()
{
	return m_data.size();
}

void NestedList::add_arc(size_t from, size_t to, float weight)
{
	check_peak(from);
	check_peak(to);
	
	m_data[from][to] = weight;
	
	if (is_oriented())
		m_back[to][from] = weight;
	else
		m_data[to][from] = weight;
}

bool NestedList::rem_arc(size_t from, size_t to)
{
	if (!has_arc(from, to))
		return false;
	
	m_data[from].erase(to);
	
	if (is_oriented())
		m_back[to].erase(from);
	else
		m_data[to].erase(from);
	
	return true;
}

bool NestedList::has_arc(size_t from, size_t to)
{
	check_peak(from);
	check_peak(to);
	
	return m_data[from].find(to) != m_data[from].end();
}

float NestedList::weight(size_t from, size_t to)
{
	if (has_arc(from, to))
		return m_data[from][to];
	else
		return NAN;
}

ArcList NestedList::outgoing(size_t peak)
{
	check_peak(peak);
	return ArcList(new NestedListBridge(m_data[peak].begin()), new NestedListBridge(m_data[peak].end()));
}

ArcList NestedList::incoming(size_t peak)
{
	check_peak(peak);
	
	if (is_oriented())
		return ArcList(new NestedListBridge(m_back[peak].begin()), new NestedListBridge(m_back[peak].end()));
	else
		return ArcList(new NestedListBridge(m_data[peak].begin()), new NestedListBridge(m_data[peak].end()));
}

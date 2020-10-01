#pragma once
#include <stdexcept>

using namespace std;

typedef struct 
{
	size_t peak;
	float weight;
} Arc;

class ArcBridge
{
	public:
	virtual void next() = 0;
	virtual bool equals(ArcBridge* other) = 0;
	virtual Arc current() = 0;
	virtual ~ArcBridge() { }
	//virtual = 0;
};

class ArcIterator
{
	private: 
	ArcBridge* m_bridge;
	
	public:
	ArcIterator(ArcBridge* bridge) { m_bridge = bridge; }
	
	ArcIterator& operator++() { m_bridge->next(); return *this; }
	bool operator==(const ArcIterator& other) { return m_bridge->equals(other.m_bridge); }
	bool operator!=(const ArcIterator& other) { return !m_bridge->equals(other.m_bridge); }
	Arc operator*() { return m_bridge->current(); }
};


class ArcList
{
	private:
	ArcBridge *m_begin;
	ArcBridge *m_end;
	
	public:
	ArcList(ArcBridge *from, ArcBridge *to)
	{
		m_begin = from;
		m_end = to;
	}
	ArcIterator begin() const { return ArcIterator(m_begin); }
	ArcIterator end() const { return ArcIterator(m_end); }
	
	~ArcList() { delete m_begin; delete m_end; }
};

class Graph
{
	private:
	bool m_oriented;
	
	protected:
	void check_peak(size_t peak)
	{
		if (peak >= this->peak_count())
			throw out_of_range("peak number is too big");
	}
	
	public:
	Graph(bool oriented) { m_oriented = oriented; }
	virtual ~Graph() { }
	bool is_oriented() { return m_oriented; }
	virtual void add_peak() = 0;
	virtual void rem_peak(size_t peak) = 0;
	virtual size_t peak_count() = 0;
	virtual void add_arc(size_t from, size_t to, float weight) = 0;
	virtual bool rem_arc(size_t from, size_t to) = 0;
	virtual bool has_arc(size_t from, size_t to) = 0;
	virtual float weight(size_t from, size_t to) = 0;
	virtual ArcList outgoing(size_t peak) = 0;
	virtual ArcList incoming(size_t peak) = 0;
};
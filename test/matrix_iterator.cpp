#include <cassert>
#include <vector>
#include <iostream>
#include <cmath>
#include "nestedmatrix.h"

using namespace std;

int main()
{
	NestedMatrix og1(5, true);
	NestedMatrix ng1(5, false);
	
	Graph& og = og1;
	Graph& ng = ng1;
	
	og.add_peak();
	og.add_arc(1, 3, 14.6);
	og.add_arc(1, 4, 12.5);
	og.add_arc(1, 5, 14.5);

	og.add_arc(2, 5, 8.6);
	
	vector<float> v1;
	
	for (auto it : og.outgoing(1))
	{
		v1.push_back(it.weight);
	}
	
	assert(v1.size() == 3);
	assert(fabs(v1[0] - 14.6) < 1e-6);
	assert(v1[1] == 12.5);
	assert(v1[2] == 14.5);
	
	vector<float> v2;
	
	for (auto it : og.incoming(5))
	{
		v2.push_back(it.weight);
	}
	assert(v2.size() == 2);
	assert(fabs(v2[0] - 14.5) < 1e-6);
	assert(fabs(v2[1] - 8.6) < 1e-6);
	
	return 0;
}
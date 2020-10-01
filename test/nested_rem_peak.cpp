#include <cassert>
#include <iostream>
#include "nestedlist.h"

using namespace std;

int main()
{
	NestedList og(5, true);
	NestedList ng(5, false);
	
	og.add_arc(1, 4, 1.0);
	og.add_arc(1, 3, 1.0);
	og.add_arc(2, 4, 1.0);
	
	og.rem_peak(2);
	
	for (auto it : og.outgoing(1))
		cout << it.peak << endl;
	cout << endl;
	
	for (auto it : og.incoming(3))
		cout << it.peak << endl;

	assert(og.peak_count() == 4);
	assert(og.has_arc(1, 3));
	assert(og.has_arc(1, 2));
	
	return 0;
}
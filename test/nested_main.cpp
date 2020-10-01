#include <cassert>
#include <cmath>
#include "nestedlist.h"

int main()
{
	NestedList og1(5, true);
	NestedList ng1(5, false);
	
	Graph& og = og1;
	Graph& ng = ng1;
	
	og.add_peak();
	ng.add_peak();
	
	assert(og.peak_count() == 6);
	assert(ng.peak_count() == 6);
	assert(og.is_oriented());
	assert(!ng.is_oriented());
	
	og.add_arc(1, 5, 20.5);
	ng.add_arc(1, 5, 20.5);
	
	assert(og.has_arc(1, 5));
	assert(!og.has_arc(5, 1));
	assert(og.weight(1, 5) == 20.5);
	assert(isnan(og.weight(5, 1)));

	assert(ng.has_arc(1, 5));
	assert(ng.has_arc(5, 1));
	assert(ng.weight(1, 5) == 20.5);
	assert(ng.weight(5, 1) == 20.5);
	
	assert(!og.rem_arc(5, 1));
	assert(ng.rem_arc(5, 1));
	
	assert(og.has_arc(1, 5));
	assert(!ng.has_arc(1, 5));
	assert(!ng.has_arc(5, 1));
	
	assert(og.rem_arc(1, 5));
	assert(!og.has_arc(1, 5));
	assert(!og.has_arc(5, 1));

	return 0;
}
#pragma once

#include <string>

namespace ee0
{

class SliderItem
{
public:
	std::string title;
	std::string name;

	int val;
	int min;
	int max;

	SliderItem(const char* title, const char* name, int val , int min, int max)
		: title(title), name(name), val(val), min(min), max(max) {}

}; // SliderItem

}
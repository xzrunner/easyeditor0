#pragma once

#include <string>
#include <vector>

namespace ee0
{

class StringHelper
{
public:
	static void Split(const std::string& src, const std::string& mid,
		std::vector<std::string>& dst);

}; // StringHelper

}
#pragma once

#include <sstream>
#include <iomanip>

namespace ee0
{

template<typename T>
std::string StringHelper::ToString(const T& d) 
{
	std::ostringstream out;
	out << d;
	return out.str();
}

template<typename T>
static std::string StringHelper::ToStringWithPrecision(const T& d, const int n)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

}
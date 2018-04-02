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

	template<typename T>
	static std::string ToString(const T& d);

	template<typename T>
	static std::string ToStringWithPrecision(const T& d, const int n = 6);

}; // StringHelper

}

#include "ee0/StringHelper.inl"
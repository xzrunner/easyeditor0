#pragma once

#include <functional>
#include <vector>

namespace ee0
{

template<class T>
class SelectionSet
{
public:
	virtual ~SelectionSet() {}

	virtual void Clear();

	virtual void Add(const T& item);
	virtual void Remove(const T& item);

	size_t Size() const;
	bool IsEmpty() const;

	bool IsExist(const T& item) const;

	void Traverse(std::function<bool(const T&)> func) const;

protected:
	std::vector<T> m_items;

}; // SelectionSet

}

#include "ee0/SelectionSet.inl"
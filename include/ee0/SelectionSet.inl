#pragma once

namespace ee0
{

template<class T>
inline void SelectionSet<T>::Clear()
{
	m_items.clear();
}

template<class T>
inline void SelectionSet<T>::Add(const T& item)
{
	if (!IsExist(item)) {
		m_items.push_back(item);
	}
}

template<class T>
inline void SelectionSet<T>::Remove(const T& item)
{
	auto itr = m_items.begin();
	for (; itr != m_items.end(); ++itr) {
		if (*itr == item) {
			m_items.erase(itr);
			break;
		}
	}
}

template<class T>
inline size_t SelectionSet<T>::Size() const
{
	return m_items.size();
}

template<class T>
inline bool SelectionSet<T>::IsEmpty() const
{
	return m_items.empty();
}

template<class T>
inline bool SelectionSet<T>::IsExist(const T& _item) const
{
	for (auto& item : m_items) {
		if (item == _item) {
			return true;
		}
	}
	return false;
}

template<class T>
inline void SelectionSet<T>::Traverse(std::function<bool(const T&)> func) const
{
	for (auto& item : m_items) {
		if (!func(item)) {
			break;
		}
	}
}

}
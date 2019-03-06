#include "ee0/Subject.h"
#include "ee0/Observer.h"
#include "ee0/VariantSet.h"

namespace ee0
{

Subject::Subject(uint32_t id)
	: m_id(id)
{
}

void Subject::RegisterObserver(Observer* o)
{
	m_observers.insert(o);
}

void Subject::UnregisterObserver(Observer* o)
{
	m_observers.erase(o);
}

void Subject::NotifyObservers(const VariantSet& variants)
{
	const Observer* skip = nullptr;
	auto var = variants.GetVariant("skip_observer");
	if (var.m_type == VT_PVOID) {
		skip = static_cast<const Observer*>(var.m_val.pv);
	}

	auto copy = m_observers;
	for (auto& o : copy)
	{
		if (skip && skip == o) {
			continue;
		}
		o->OnNotify(m_id, variants);
	}
}

}
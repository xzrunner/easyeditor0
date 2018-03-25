#pragma once

#include "ee0/Subject.h"
#include "ee0/VariantSet.h"

#include <map>
#include <memory>

namespace ee0
{

class SubjectMgr
{
public:
	void RegisterObserver(uint32_t msg, Observer* o);
	bool UnregisterObserver(uint32_t msg, Observer* o);

	bool NotifyObservers(uint32_t msg, 
		const VariantSet& variants = VariantSet());

private:
	Subject* QuerySubject(uint32_t id) const;

private:
	std::map<uint32_t, std::unique_ptr<Subject>> m_subjects;

}; // SubjectMgr

}
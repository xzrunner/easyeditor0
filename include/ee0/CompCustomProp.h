#pragma once

#include "ee0/Variant.h"

#include <node0/NodeUniqueComp.h>

#include <string>
#include <vector>

namespace ee0
{

class CompCustomProp : public n0::NodeUniqueComp
{
public:
	enum PropertyType
	{
		PROP_BOOL,
		PROP_INT,
		PROP_FLOAT,
		PROP_STRING,
		PROP_VEC2,
		PROP_COLOR,
	};

	struct Property
	{
		PropertyType type;
		Variant      val;
		std::string  key;
	};

public:
	CompCustomProp();

	virtual const char* Type() const override { return TYPE_NAME; }
	virtual n0::UniqueCompID TypeID() const override {
		return n0::GetUniqueCompTypeID<CompCustomProp>();
	}
	virtual std::unique_ptr<n0::NodeUniqueComp> Clone(const n0::SceneNode& obj) const override;

	void Add(const Property& prop);
	void Remove(const std::string& name);

	auto& GetAllProp() const { return m_props; }
	auto& GetAllProp() { return m_props; }

	static const char* const TYPE_NAME;

private:
	std::vector<Property> m_props;

}; // CompCustomProp

}
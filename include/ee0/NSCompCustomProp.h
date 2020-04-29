#pragma once

#include "ee0/CompCustomProp.h"

#include <ns/NodeComponent.h>

#include <vector>

namespace ee0
{

class NSCompCustomProp : public ns::NodeComponent
{
public:
	NSCompCustomProp() {}

	//
	// bin
	//
	virtual size_t GetBinSize(const std::string& dir) const override;
	virtual void   StoreToBin(const std::string& dir, bs::ExportStream& es) const override;
	virtual void   LoadFromBin(const ur::Device& dev,
        const std::string& dir, bs::ImportStream& is) override;

	//
	// json
	//
	virtual void StoreToJson(const std::string& dir, rapidjson::Value& val, rapidjson::MemoryPoolAllocator<>& alloc) const override;
	virtual void LoadFromJson(const ur::Device& dev,
        mm::LinearAllocator& alloc, const std::string& dir, const rapidjson::Value& val) override;

	//
	// memory
	//
	void StoreToMem(const ur::Device& dev, ee0::CompCustomProp& comp) const;
	void LoadFromMem(const ee0::CompCustomProp& comp);

private:
	std::vector<ee0::CompCustomProp::Property> m_props;

}; // NSCompCustomProp

}
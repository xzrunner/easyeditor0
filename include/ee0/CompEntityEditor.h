#pragma once

#include <entity0/Component.h>

#include <string>
#include <memory>

namespace ee0
{

struct CompEntityEditor : public e0::Component
{
	std::shared_ptr<std::string> filepath = nullptr;
	std::shared_ptr<std::string> name = nullptr;

	bool visible, editable;

}; // CompEntityEditor

}
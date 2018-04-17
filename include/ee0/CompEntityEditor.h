#pragma once

#include <ecsx/Component.h>

#include <string>
#include <memory>

namespace ee0
{

struct CompEntityEditor : public ecsx::Component
{
	std::shared_ptr<std::string> filepath = nullptr;
	std::shared_ptr<std::string> name = nullptr;

	bool visible, editable;

}; // CompEntityEditor

}
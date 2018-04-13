#pragma once

#include <ecsx/Component.h>

#include <string>

namespace ee0
{

struct CompEntityEditor : public ecsx::Component
{
	std::string filepath;
	std::string name;

	bool visible, editable;

}; // CompEntityEditor

}
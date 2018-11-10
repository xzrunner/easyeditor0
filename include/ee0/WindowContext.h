#pragma once

#include <memory>

namespace pt2 { class WindowContext; }
namespace pt3 { class WindowContext; }
namespace egui { struct Context; }

namespace ee0
{

class WindowContext
{
public:
	std::shared_ptr<pt2::WindowContext> wc2 = nullptr;
	std::shared_ptr<pt3::WindowContext> wc3 = nullptr;

	std::shared_ptr<egui::Context> egui = nullptr;

}; // WindowContext

}
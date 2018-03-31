#pragma once

#include <memory>

class wxGLContext;

namespace facade { class RenderContext; }

namespace ee0
{

class RenderContext
{
public:
	std::shared_ptr<wxGLContext>           gl_ctx = nullptr;
	std::shared_ptr<facade::RenderContext> facade_rc = nullptr;

}; // RenderContext

}
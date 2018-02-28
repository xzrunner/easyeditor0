#pragma once

#include <memory>

class wxGLContext;

namespace gum { class RenderContext; }

namespace ee0
{

class RenderContext
{
public:
	std::shared_ptr<wxGLContext>        gl_ctx = nullptr;
	std::shared_ptr<gum::RenderContext> gum_rc = nullptr;

}; // RenderContext

}
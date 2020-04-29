#pragma once

#include <unirender/RenderState.h>

#include <memory>

class wxGLContext;

namespace ur { class Context; }
namespace facade { class RenderContext; }

namespace ee0
{

class RenderContext
{
public:
    std::shared_ptr<ur::Context> ur_ctx = nullptr;
    ur::RenderState ur_rs;

	std::shared_ptr<wxGLContext>           gl_ctx = nullptr;
	//std::shared_ptr<facade::RenderContext> facade_rc = nullptr;

	static void Reset2D(bool clear = false);
	static void Reset3D(bool clear = false);

}; // RenderContext

}
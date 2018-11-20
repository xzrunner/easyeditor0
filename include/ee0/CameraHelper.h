#pragma once

namespace pt0 { class Camera; }

#include <SM_Vector.h>

namespace ee0
{

class CameraHelper
{
public:
	static sm::vec2 TransPosScreenToProject(const pt0::Camera& cam, int screen_x, int screen_y);
	static sm::vec2 TransPosProjectToScreen(const pt0::Camera& cam, const sm::vec2& project);

	static sm::ivec2 GetScreenSize(const pt0::Camera& cam);

}; // CameraHelper

}
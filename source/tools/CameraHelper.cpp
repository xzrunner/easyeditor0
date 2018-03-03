#include "ee0/CameraHelper.h"

#include <painting2/OrthoCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/WindowContext.h>
#include <gum/Blackboard.h>

namespace ee0
{

sm::vec2 CameraHelper::TransPosScreenToProject(const pt2::Camera& cam, int screen_x, int screen_y)
{
	if (cam.Type() != pt2::CAM_ORTHO2D) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	return ortho_cam.TransPosScreenToProject(
		screen_x, screen_y, wc->GetScreenWidth(), wc->GetScreenHeight());
}

sm::vec2 CameraHelper::TransPosProjectToScreen(const pt2::Camera& cam, const sm::vec2& project)
{
	if (cam.Type() != pt2::CAM_ORTHO2D) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	return ortho_cam.TransPosProjectToScreen(
		project, wc->GetScreenWidth(), wc->GetScreenHeight());
}

}
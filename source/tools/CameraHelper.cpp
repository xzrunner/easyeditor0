#include "ee0/CameraHelper.h"

#include <painting2/OrthoCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/WindowContext.h>

namespace ee0
{

sm::vec2 CameraHelper::TransPosScreenToProject(const pt0::Camera& cam, int screen_x, int screen_y)
{
	if (cam.TypeID() != pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	return ortho_cam.TransPosScreenToProject(
		screen_x, screen_y, wc->GetScreenWidth(), wc->GetScreenHeight());
}

sm::vec2 CameraHelper::TransPosProjectToScreen(const pt0::Camera& cam, const sm::vec2& project)
{
	if (cam.TypeID() != pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		return sm::vec2(0, 0);
	}

	auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	return ortho_cam.TransPosProjectToScreen(
		project, wc->GetScreenWidth(), wc->GetScreenHeight());
}

}
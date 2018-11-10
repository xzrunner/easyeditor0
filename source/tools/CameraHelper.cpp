#include "ee0/CameraHelper.h"
#include "ee0/WindowContext.h"

#include <painting2/Blackboard.h>
#include <painting2/OrthoCamera.h>
#include <painting2/WindowContext.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>

namespace ee0
{

sm::vec2 CameraHelper::TransPosScreenToProject(const pt0::Camera& cam, int screen_x, int screen_y)
{
	auto type = cam.TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
		auto wc = pt2::Blackboard::Instance()->GetWindowContext();
		auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
		return ortho_cam.TransPosScreenToProject(
			screen_x, screen_y, wc->GetScreenWidth(), wc->GetScreenHeight());
	}
	else
	{
		auto wc = pt3::Blackboard::Instance()->GetWindowContext();
		auto sz = wc->GetScreenSize();
		return sm::vec2(screen_x - sz.x * 0.5f, sz.y * 0.5f - screen_y);
	}
}

sm::vec2 CameraHelper::TransPosProjectToScreen(const pt0::Camera& cam, const sm::vec2& project)
{
	auto type = cam.TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
		auto wc = pt2::Blackboard::Instance()->GetWindowContext();
		auto& ortho_cam = dynamic_cast<const pt2::OrthoCamera&>(cam);
		return ortho_cam.TransPosProjectToScreen(
			project, wc->GetScreenWidth(), wc->GetScreenHeight());
	}
	else
	{
		auto wc = pt3::Blackboard::Instance()->GetWindowContext();
		auto sz = wc->GetScreenSize();
		return sm::vec2(project.x + sz.x * 0.5f, project.y - sz.y * 0.5f);
	}
}

}
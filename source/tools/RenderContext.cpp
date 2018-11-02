#include "ee0/RenderContext.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

namespace ee0
{

void RenderContext::Reset2D(bool clear)
{	
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	if (clear) {
		ur_rc.SetClearFlag(ur::MASKC);
		ur_rc.SetClearColor(0x88888888);
		ur_rc.Clear();
	}
	ur_rc.SetDepthTest(ur::DEPTH_DISABLE);
//	ur_rc.EnableDepthMask(false);
	ur_rc.SetCull(ur::CULL_DISABLE);
}

void RenderContext::Reset3D(bool clear)
{
	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	if (clear) {
		ur_rc.SetClearFlag(ur::MASKC | ur::MASKD);
		ur_rc.SetClearColor(0x88888888);
		ur_rc.Clear();
	}
	ur_rc.SetDepthTest(ur::DEPTH_LESS_EQUAL);
	ur_rc.EnableDepthMask(true);
	ur_rc.SetFrontFace(true);
	ur_rc.SetCull(ur::CULL_BACK);
}

}
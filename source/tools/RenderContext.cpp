#include "ee0/RenderContext.h"

namespace ee0
{

void RenderContext::Reset2D(bool clear)
{
//	auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
//	if (clear) {
//		ur_rc.SetClearFlag(ur::MASKC);
//		ur_rc.SetClearColor(0x88888888);
//		ur_rc.Clear();
//	}
//	ur_rc.SetZTest(ur::DEPTH_DISABLE);
////	ur_rc.SetZWrite(false);
//	ur_rc.SetCullMode(ur::CULL_DISABLE);
}

void RenderContext::Reset3D(bool clear)
{
	//auto& ur_rc = ur::Blackboard::Instance()->GetRenderContext();
	//if (clear) {
	//	ur_rc.SetClearFlag(ur::MASKC | ur::MASKD);
	//	ur_rc.SetClearColor(0x88888888);
	//	ur_rc.Clear();
	//}
	//ur_rc.SetZTest(ur::DEPTH_LESS_EQUAL);
	//ur_rc.SetZWrite(true);
	//ur_rc.SetFrontFace(true);
	//ur_rc.SetCullMode(ur::CULL_BACK);
}

}
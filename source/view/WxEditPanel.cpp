#include "ee0/WxEditPanel.h"
#include "ee0/EditPanelImpl.h"
#include "ee0/SubjectMgr.h"

#include <moon/Context.h>
#include <moon/Blackboard.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxEditPanel, wxPanel)
 	EVT_SIZE(WxEditPanel::OnSize)
END_EVENT_TABLE()

WxEditPanel::WxEditPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr)
	: wxPanel(parent)
	, m_sub_mgr(sub_mgr)
{
	if (!m_sub_mgr) {
		m_sub_mgr = std::make_shared<SubjectMgr>();
	}
	m_impl = std::make_unique<EditPanelImpl>(parent, m_sub_mgr);

	m_moon_ctx = std::make_shared<moon::Context>();
	moon::Blackboard::Instance()->SetContext(m_moon_ctx);
	m_moon_ctx->Init();
}

bool WxEditPanel::GetKeyState(int key) const
{
	return m_impl->GetKeyState(key);
}

void WxEditPanel::OnSize(wxSizeEvent& event)
{
	m_impl->OnSize(event);
}

}
#pragma once

#include "ee0/EditPanelImpl.h"
#include "ee0/typedef.h"

#include <moon/typedef.h>

#include <wx/wx.h>

#include <memory>

namespace ee0
{

class WxEditPanel : public wxPanel
{
public:
	WxEditPanel(wxWindow* parent, const SubjectMgrPtr& sub_mgr);
	~WxEditPanel() = default;
	const SubjectMgrPtr& GetSubjectMgr() const { return m_sub_mgr; }

	EditPanelImpl& GetImpl() { return *m_impl; }
	const EditPanelImpl& GetImpl() const { return *m_impl; }

	bool GetKeyState(int key) const;

	const moon::ContextPtr& GetMoonCtx() const { return m_moon_ctx; }

private:
	void OnSize(wxSizeEvent& event);

protected:
	SubjectMgrPtr m_sub_mgr = nullptr;

private:
	std::unique_ptr<EditPanelImpl> m_impl;

	moon::ContextPtr m_moon_ctx = nullptr;

	DECLARE_EVENT_TABLE()

}; // WxEditPanel

}
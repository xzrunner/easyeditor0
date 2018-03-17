#pragma once

#include "ee0/EditPanelImpl.h"
#include "ee0/typedef.h"

#include <wx/wx.h>

#include <memory>

namespace dust { class LuaVM; }

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

	const std::shared_ptr<dust::LuaVM>& GetLuaVM() const { return m_vm; }

private:
	void OnSize(wxSizeEvent& event);

protected:
	SubjectMgrPtr m_sub_mgr = nullptr;

private:
	std::unique_ptr<EditPanelImpl> m_impl;

	std::shared_ptr<dust::LuaVM> m_vm = nullptr;

	DECLARE_EVENT_TABLE()

}; // WxEditPanel

}
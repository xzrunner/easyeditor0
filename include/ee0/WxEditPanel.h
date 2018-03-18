#pragma once

#include "ee0/EditPanelImpl.h"
#include "ee0/typedef.h"

#include <dust/typedef.h>

#include <wx/wx.h>

#include <memory>

namespace dust { class LuaVM;}

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

	const dust::LuaVMPtr& GetLuaVM() const { return m_lua; }

private:
	void OnSize(wxSizeEvent& event);

protected:
	SubjectMgrPtr m_sub_mgr = nullptr;

private:
	std::unique_ptr<EditPanelImpl> m_impl;

	dust::LuaVMPtr m_lua = nullptr;

	DECLARE_EVENT_TABLE()

}; // WxEditPanel

}
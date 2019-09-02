#pragma once

#include "ee0/typedef.h"

#include <wx/wx.h>

#include <memory>

namespace ee0
{

class EditOP;
class WxStageCanvas;
class RightPopupMenu;

class EditPanelImpl
{
public:
	EditPanelImpl(wxWindow* parent, const SubjectMgrPtr& sub_mgr);

	const std::shared_ptr<EditOP>& GetEditOP() const { return m_edit_op; }
	std::shared_ptr<EditOP>& GetEditOP() { return m_edit_op; }
	void SetEditOP(const std::shared_ptr<EditOP>& op);

	const std::shared_ptr<WxStageCanvas>& GetCanvas() const { return m_canvas; }
	std::shared_ptr<WxStageCanvas>& GetCanvas() { return m_canvas; }
	void SetCanvas(const std::shared_ptr<WxStageCanvas>& canvas) { m_canvas = canvas; }

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);
	void OnRightPopupMenu(wxCommandEvent& event);

    void  SetPopupMenu(const std::shared_ptr<RightPopupMenu>& popup) { m_popup = popup; }
    auto& GetPopupMenu() const { return m_popup; }

	void OnSize(wxSizeEvent& event);

	void SetOnKeyDownFunc(std::function<void(int)> func) {
		m_on_key_down_func = func;
	}

private:
	SubjectMgrPtr m_sub_mgr;

	std::shared_ptr<EditOP> m_edit_op = nullptr;

	std::shared_ptr<WxStageCanvas> m_canvas = nullptr;

	std::shared_ptr<RightPopupMenu> m_popup = nullptr;

	std::function<void(int)> m_on_key_down_func = nullptr;

}; // EditPanelImpl

}
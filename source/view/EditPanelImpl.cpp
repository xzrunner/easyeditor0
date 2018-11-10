#include "ee0/EditPanelImpl.h"
#include "ee0/EditOP.h"
#include "ee0/WxStageCanvas.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MsgHelper.h"
#include "ee0/MessageID.h"
#include "ee0/CameraHelper.h"

#include <easygui/InputEvent.h>
#include <easygui/Utility.h>
#include <facade/EasyGUI.h>

namespace ee0
{

EditPanelImpl::EditPanelImpl(wxWindow* parent, const SubjectMgrPtr& sub_mgr)
	: m_sub_mgr(sub_mgr)
	, m_popup(parent, this)
{
}

void EditPanelImpl::SetEditOP(const std::shared_ptr<EditOP>& op)
{
	if (m_edit_op == op) {
		return;
	}

	m_edit_op = op;

	m_edit_op->OnActive();
}

void EditPanelImpl::OnMouse(wxMouseEvent& event)
{
	if (!m_edit_op) {
		return;
	}

	auto& egui = std::const_pointer_cast<egui::Context>(m_canvas->GetWidnowContext().egui);

	const int x = event.GetX();
	const int y = event.GetY();
	GetCanvas()->GetWidnowContext();
	auto proj = CameraHelper::TransPosScreenToProject(*GetCanvas()->GetCamera(), x, y);
	if (event.LeftDown())
	{
		m_edit_op->OnMouseLeftDown(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_LEFT_DOWN, proj.x, proj.y);
		}
	}
	else if (event.LeftUp())
	{
		m_edit_op->OnMouseLeftUp(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_LEFT_UP, proj.x, proj.y);
		}
	}
	else if (event.RightDown())
	{
		m_edit_op->OnMouseRightDown(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_RIGHT_DOWN, proj.x, proj.y);
		}
	}
	else if (event.RightUp())
	{
		m_edit_op->OnMouseRightUp(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_RIGHT_UP, proj.x, proj.y);
		}
	}
	else if (event.Moving())
	{
		m_edit_op->OnMouseMove(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_MOVE, proj.x, proj.y);
		}
	}
	else if (event.Dragging())
	{
		m_edit_op->OnMouseDrag(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_DRAG, proj.x, proj.y);
		}
	}
	else if (event.LeftDClick())
	{
		m_edit_op->OnMouseLeftDClick(x, y);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_LEFT_DCLICK, proj.x, proj.y);
		}
	}
	else if (event.GetWheelRotation())
	{
		int dir = event.GetWheelRotation();
		m_edit_op->OnMouseWheelRotation(x, y, dir);
		if (egui) {
			egui->input_events.emplace_back(egui::InputType::MOUSE_WHEEL_ROTATION, proj.x, proj.y, dir);
		}
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void EditPanelImpl::OnKeyDown(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);

	if (m_on_key_down_func) {
		m_on_key_down_func(key_code);
	}

	if (GetKeyState(WXK_CONTROL) && key_code == 'Z')
	{
		m_sub_mgr->NotifyObservers(ee0::MSG_EDIT_OP_UNDO);
	}
	else if (GetKeyState(WXK_CONTROL) && key_code == 'Y'
		  || GetKeyState(WXK_CONTROL) && GetKeyState(WXK_SHIFT) && key_code == 'Y')
	{
		m_sub_mgr->NotifyObservers(ee0::MSG_EDIT_OP_REDO);
	}

	switch (key_code)
	{
	case WXK_F5:
		m_sub_mgr->NotifyObservers(ee0::MSG_EDITOR_RELOAD);
		break;
	}

	if (m_edit_op && !m_edit_op->OnKeyDown(key_code)) {
		event.Skip();
	}
}

void EditPanelImpl::OnKeyUp(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyUp(key_code);

	if (m_edit_op) {
		m_edit_op->OnKeyUp(key_code);
	}
}

void EditPanelImpl::OnChar(wxKeyEvent& event)
{
	int key_code = event.GetKeyCode();
	m_keys_state.OnKeyDown(key_code);

	if (m_edit_op) {
		m_edit_op->OnChar(key_code);
	}
}

void EditPanelImpl::OnMouseWheelRotation(int x, int y, int direction)
{
}

void EditPanelImpl::OnRightPopupMenu(wxCommandEvent& event)
{
	if (m_edit_op) {
		m_edit_op->OnPopMenuSelected(event.GetId());
	}
}

void EditPanelImpl::OnSize(wxSizeEvent& event)
{
	if (m_canvas) {
		m_canvas->SetSize(event.GetSize());
	}
}

}
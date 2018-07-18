#include "ee0/EditPanelImpl.h"
#include "ee0/EditOP.h"
#include "ee0/WxStageCanvas.h"
#include "ee0/SubjectMgr.h"
#include "ee0/MsgHelper.h"
#include "ee0/MessageID.h"

namespace ee0
{

EditPanelImpl::EditPanelImpl(wxWindow* parent, const SubjectMgrPtr& sub_mgr)
	: m_sub_mgr(sub_mgr)
	, m_popup(parent, this)
{
}

void EditPanelImpl::OnMouse(wxMouseEvent& event)
{
	if (!m_edit_op) return;

	if (event.LeftDown())
		m_edit_op->OnMouseLeftDown(event.GetX(), event.GetY());
	else if (event.LeftUp())
		m_edit_op->OnMouseLeftUp(event.GetX(), event.GetY());
	else if (event.RightDown())
		m_edit_op->OnMouseRightDown(event.GetX(), event.GetY());
	else if (event.RightUp())
		m_edit_op->OnMouseRightUp(event.GetX(), event.GetY());
	else if (event.Moving())
		m_edit_op->OnMouseMove(event.GetX(), event.GetY());
	else if (event.Dragging())
		m_edit_op->OnMouseDrag(event.GetX(), event.GetY());
	else if (event.LeftDClick())
		m_edit_op->OnMouseLeftDClick(event.GetX(), event.GetY());
	else if (event.GetWheelRotation())
		m_edit_op->OnMouseWheelRotation(event.GetX(), event.GetY(), event.GetWheelRotation());
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
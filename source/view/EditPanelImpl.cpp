#include "ee0/EditPanelImpl.h"
#include "ee0/EditOP.h"
#include "ee0/WxStageCanvas.h"
#include "ee0/SubjectMgr.h"

namespace ee0
{

EditPanelImpl::EditPanelImpl(SubjectMgr& sub_mgr)
	: m_sub_mgr(sub_mgr)
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

	if (GetKeyState(WXK_CONTROL) && key_code == 'Z') 
	{
		bool dirty = m_edit_record.Undo();

		ee0::VariantSet vars;
		ee0::Variant var;
		var.m_type = ee0::VT_BOOL;
		var.m_val.bl = dirty;
		vars.SetVariant("dirty", var);
		m_sub_mgr.NotifyObservers(MSG_SET_EDITOR_DIRTY, vars);		
	} 
	else if (GetKeyState(WXK_CONTROL) && key_code == 'Y') 
	{
		bool dirty = m_edit_record.Redo();

		ee0::VariantSet vars;
		ee0::Variant var;
		var.m_type = ee0::VT_BOOL;
		var.m_val.bl = dirty;
		vars.SetVariant("dirty", var);
		m_sub_mgr.NotifyObservers(MSG_SET_EDITOR_DIRTY, vars);
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

void EditPanelImpl::OnSize(wxSizeEvent& event)
{
	if (m_canvas) {
		m_canvas->SetSize(event.GetSize());
	}
}

}
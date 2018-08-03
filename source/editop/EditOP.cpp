#include "ee0/EditOP.h"

namespace ee0
{

bool EditOP::OnKeyDown(int key_code)
{
	if (m_prev_op && m_prev_op->OnKeyDown(key_code)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnKeyUp(int key_code)
{
	if (m_prev_op && m_prev_op->OnKeyUp(key_code)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnChar(int key_code)
{
	if (m_prev_op && m_prev_op->OnChar(key_code)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftDown(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftUp(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftUp(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseRightDown(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseRightDown(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseRightUp(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseRightUp(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseMove(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseMove(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseDrag(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseLeftDClick(int x, int y)
{
	if (m_prev_op && m_prev_op->OnMouseLeftDClick(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (m_prev_op && m_prev_op->OnMouseWheelRotation(x, y, direction)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnPopMenuSelected(int type)
{
	if (m_prev_op && m_prev_op->OnPopMenuSelected(type)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnActive()
{
	if (m_prev_op && m_prev_op->OnActive()) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::OnDraw() const
{
	if (m_prev_op && m_prev_op->OnDraw()) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::Update(float dt)
{
	if (m_prev_op && m_prev_op->Update(dt)) {
		return true;
	} else {
		return false;
	}
}

bool EditOP::Clear()
{
	if (m_prev_op && m_prev_op->Clear()) {
		return true;
	} else {
		return false;
	}
}

void EditOP::SetCamera(const std::shared_ptr<pt0::Camera>& camera)
{
	if (m_camera == camera) {
		return;
	}

	m_camera = camera;

	auto prev = m_prev_op;
	while (prev) {
		prev->SetCamera(camera);
		prev = prev->m_prev_op;
	}

	if (m_op_state) {
		m_op_state->SetCamera(camera);
	}
}

void EditOP::ChangeEditOpState(const std::shared_ptr<EditOpState>& state)
{
	if (m_op_state == state) {
		return;
	}

	if (m_op_state) {
		m_op_state->UnBind();
	}
	m_op_state = state;
	if (m_op_state) {
		m_op_state->Bind();
	}

	m_op_state->SetCamera(m_camera);
}

}
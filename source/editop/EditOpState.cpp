#include "ee0/EditOpState.h"

namespace ee0
{

void EditOpState::Bind()
{
    if (m_prev_st) {
        m_prev_st->Bind();
    }
}

void EditOpState::UnBind()
{
    if (m_prev_st) {
        m_prev_st->UnBind();
    }
}

bool EditOpState::OnKeyPress(int key_code)
{
	if (m_prev_st && m_prev_st->OnKeyPress(key_code)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnKeyRelease(int key_code)
{
	if (m_prev_st && m_prev_st->OnKeyRelease(key_code)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnMousePress(int x, int y)
{
	if (m_prev_st && m_prev_st->OnMousePress(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnMouseRelease(int x, int y)
{
	if (m_prev_st && m_prev_st->OnMouseRelease(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnMouseMove(int x, int y)
{
	if (m_prev_st && m_prev_st->OnMouseMove(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnMouseDrag(int x, int y)
{
	if (m_prev_st && m_prev_st->OnMouseDrag(x, y)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnDirectionKeyDown(int type)
{
	if (m_prev_st && m_prev_st->OnDirectionKeyDown(type)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnMouseWheelRotation(int x, int y, int direction)
{
	if (m_prev_st && m_prev_st->OnMouseWheelRotation(x, y, direction)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnActive(bool active)
{
	if (m_prev_st && m_prev_st->OnActive(active)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::OnDraw(const ur2::Device& dev, ur2::Context& ctx) const
{
	if (m_prev_st && m_prev_st->OnDraw(dev, ctx)) {
		return true;
	} else {
		return false;
	}
}

bool EditOpState::Clear()
{
	if (m_prev_st && m_prev_st->Clear()) {
		return true;
	} else {
		return false;
	}
}

}
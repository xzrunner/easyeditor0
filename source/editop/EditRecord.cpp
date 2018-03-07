#include "ee0/EditRecord.h"
#include "ee0/AtomicOP.h"

#include <guard/check.h>

namespace ee0
{

void EditRecord::Add(const std::shared_ptr<AtomicOP>& ao)
{
	GD_ASSERT(ao, "ao null");
	m_undo_stack.push(ao);
	Clear(m_redo_stack);
}

void EditRecord::Clear()
{
	Clear(m_undo_stack);
	Clear(m_redo_stack);
	m_saved_op.reset();
}

bool EditRecord::Undo()
{
	if (m_undo_stack.empty()) {
		return false;
	}

	auto ao = m_undo_stack.top();
	m_undo_stack.pop();
	ao->Undo();
	m_redo_stack.push(ao);

	if (!m_saved_op && m_undo_stack.empty()) {
		return false;
	} else if (m_saved_op && !m_undo_stack.empty() && m_saved_op == m_undo_stack.top()) {
		return false;
	} else {
		return true;
	}
}

bool EditRecord::Redo()
{
	if (m_redo_stack.empty()) {
		return false;
	}

	auto op = m_redo_stack.top();
	m_redo_stack.pop();
	op->Redo();
	m_undo_stack.push(op);

	if (!m_saved_op && m_undo_stack.empty()) {
		return false;
	} else if (m_saved_op && !m_undo_stack.empty() && m_saved_op == m_undo_stack.top()) {
		return false;
	} else {
		return true;
	}
}

void EditRecord::OnSave()
{
	if (!m_undo_stack.empty()) {
		m_saved_op = m_undo_stack.top();
	}
}

void EditRecord::Clear(std::stack<std::shared_ptr<AtomicOP>>& stack)
{
	while (!stack.empty()) {
		stack.pop();
	}
}

}
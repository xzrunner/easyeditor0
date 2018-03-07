#pragma once

#include <stack>
#include <memory>

namespace ee0
{

class AtomicOP;

class EditRecord
{
public:
	void Add(const std::shared_ptr<AtomicOP>& ao);
	void Clear();

	bool Undo();
	bool Redo();

	void OnSave();

private:
	static void Clear(std::stack<std::shared_ptr<AtomicOP>>& stack);

private:
	std::stack<std::shared_ptr<AtomicOP>> m_undo_stack, m_redo_stack;

	std::shared_ptr<AtomicOP> m_saved_op = nullptr;

}; // EditRecord

}
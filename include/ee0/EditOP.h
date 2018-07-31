#pragma once

#include <memory>

namespace ee0
{

class EditOP
{
public:
	virtual ~EditOP() {}

	virtual bool OnKeyDown(int key_code);
	virtual bool OnKeyUp(int key_code);
	virtual bool OnChar(int key_code);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);
	virtual bool OnMouseWheelRotation(int x, int y, int direction);

	virtual bool OnPopMenuSelected(int type);

	virtual bool OnActive();
	virtual bool OnDraw() const;
	virtual bool Update(float dt);
	virtual bool Clear();

	auto& SetPrevEditOP(const std::shared_ptr<EditOP>& op) {
		m_prev_op = op;
		return op;
	}

private:
	std::shared_ptr<EditOP> m_prev_op = nullptr;

}; // EditOP

}
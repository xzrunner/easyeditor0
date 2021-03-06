#pragma once

#include "ee0/EditOpState.h"

#include <memory>

namespace ur { class Device; class Context; }

namespace ee0
{

class EditOpState;

class EditOP
{
public:
	EditOP(const std::shared_ptr<pt0::Camera>& camera)
		: m_camera(camera)
	{
	}
	virtual ~EditOP() {}

	virtual bool OnKeyDown(int key_code);
	virtual bool OnKeyUp(int key_code);
	virtual bool OnChar(int key_code);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
    virtual bool OnMouseMiddleDown(int x, int y);
    virtual bool OnMouseMiddleUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);
	virtual bool OnMouseWheelRotation(int x, int y, int direction);

	virtual bool OnPopMenuSelected(int type);

	virtual bool OnActive();
	virtual bool OnDraw(const ur::Device& dev, ur::Context& ctx) const;
	virtual bool Update(float dt);
	virtual bool Clear();

	virtual void SetCamera(const std::shared_ptr<pt0::Camera>& camera);

    std::shared_ptr<EditOP>& SetPrevEditOP(const std::shared_ptr<EditOP>& op);
    void AddPrevEditOP(const std::shared_ptr<EditOP>& op);

protected:
	void ChangeEditOpState(const std::shared_ptr<EditOpState>& state);

protected:
	std::shared_ptr<pt0::Camera> m_camera = nullptr;

	std::shared_ptr<EditOpState> m_op_state = nullptr;

private:
	std::shared_ptr<EditOP> m_prev_op = nullptr;

}; // EditOP

}
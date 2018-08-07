#pragma once

#include <memory>

namespace pt0 { class Camera; }

namespace ee0
{

class EditOpState
{
public:
	EditOpState(const std::shared_ptr<pt0::Camera>& camera)
		: m_camera(camera) {}
	virtual ~EditOpState() {}

	virtual void Bind() {}
	virtual void UnBind() {}

	virtual bool OnKeyPress(int key_code) { return false; }
	virtual bool OnKeyRelease(int key_code) { return false; }
	virtual bool OnMousePress(int x, int y) { return false; }
	virtual bool OnMouseRelease(int x, int y) { return false; }
	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDrag(int x, int y) { return false; }

	virtual bool OnDirectionKeyDown(int type) { return false; }

	virtual bool OnMouseWheelRotation(int x, int y, int direction) { return false; }

	virtual bool OnActive() { return false; };
	virtual bool OnDraw() const { return false; };
	virtual bool Clear() { return false; }

	void SetCamera(const std::shared_ptr<pt0::Camera>& camera) {
		m_camera = camera;
	}

protected:
	std::shared_ptr<pt0::Camera> m_camera = nullptr;

}; // EditOpState

}
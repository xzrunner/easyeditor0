#pragma once

#include <memory>

namespace ur2 { class Device; class Context; }
namespace pt0 { class Camera; }

namespace ee0
{

class EditOpState
{
public:
	EditOpState(const std::shared_ptr<pt0::Camera>& camera)
		: m_camera(camera) {}
	virtual ~EditOpState() {}

    virtual void Bind();
    virtual void UnBind();

    virtual bool OnKeyPress(int key_code);
    virtual bool OnKeyRelease(int key_code);
    virtual bool OnMousePress(int x, int y);
    virtual bool OnMouseRelease(int x, int y);
    virtual bool OnMouseMove(int x, int y);
    virtual bool OnMouseDrag(int x, int y);

    virtual bool OnDirectionKeyDown(int type);

    virtual bool OnMouseWheelRotation(int x, int y, int direction);

	virtual bool OnActive(bool active);
	virtual bool OnDraw(const ur2::Device& dev, ur2::Context& ctx) const;
    virtual bool Clear();

	void SetCamera(const std::shared_ptr<pt0::Camera>& camera) {
		m_camera = camera;
	}

    auto& SetPrevOpState(const std::shared_ptr<EditOpState>& st) {
        m_prev_st = st;
        return st;
    }

protected:
	std::shared_ptr<pt0::Camera> m_camera = nullptr;

private:
    std::shared_ptr<EditOpState> m_prev_st = nullptr;

}; // EditOpState

}
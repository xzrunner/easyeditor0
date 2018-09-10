#pragma once

#include "ee0/RenderContext.h"
#include "ee0/WindowContext.h"

#include <painting0/Camera.h>

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include <memory>
#include <functional>

namespace ee0
{

class EditPanelImpl;

class WxStageCanvas : public wxGLCanvas
{
public:
	static const uint32_t HAS_2D            = 0x00000001;
	static const uint32_t HAS_3D            = 0x00000002;

public:
	WxStageCanvas(wxWindow* wnd, EditPanelImpl& stage, const pt0::CameraPtr& camera,
		const RenderContext* rc = nullptr, const WindowContext* wc = nullptr,
		uint32_t flag = HAS_2D);
	virtual ~WxStageCanvas();

	const RenderContext& GetRenderContext() const { return m_rc; }
	const WindowContext& GetWidnowContext() const { return m_wc; }

	pt0::CameraPtr& GetCamera() { return m_camera; }
	const pt0::CameraPtr& GetCamera() const { return m_camera; }
	void SetCamera(const pt0::CameraPtr& camera) { m_camera = camera; }

	void SetDirty() { m_dirty = true; }

	void AddUpdateTask(std::function<void()> task) { m_tasks.push_back(task); }

	static wxGLCanvas* CreateWxGLCanvas(wxWindow* wnd);
	static void CreateRenderContext(RenderContext& rc, wxGLCanvas* canvas);
	static void CreateWindowContext(WindowContext& wc, bool has2d, bool has3d);

protected:
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDrawWhole() const;
	virtual void OnDrawSprites() const = 0;

	virtual void OnTimer() {}
	virtual bool OnUpdate() { return false; }

	virtual void OnMouseImpl(wxMouseEvent& event) {}
	virtual void OnKeyDownImpl(wxKeyEvent& event) {}

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	//void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);

	void OnTimer(wxTimerEvent& event);

	void OnKillFocus(wxFocusEvent& event);

	void DebugDraw() const;

protected:
	void SetCurrentCanvas();

private:
	void InitRender(const RenderContext* rc);
	void InitWindow(const WindowContext* wc);

	void BindRenderContext();

protected:
	pt0::CameraPtr m_camera = nullptr;

private:
	uint32_t m_flag;

	EditPanelImpl& m_stage;

	// context
	bool m_new_rc;
	RenderContext m_rc;
	WindowContext m_wc;

	wxTimer m_timer;
	clock_t m_last_time;

	bool m_dirty;

	std::vector<std::function<void()>> m_tasks;

	DECLARE_EVENT_TABLE()

}; // WxStageCanvas

}
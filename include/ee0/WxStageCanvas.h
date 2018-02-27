#pragma once

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include <memory>

namespace gum { class RenderContext; }

namespace ee0
{

class EditPanelImpl;

class WxStageCanvas : public wxGLCanvas
{
public:
	static const uint32_t USE_CONTEXT_STACK = 0x00000001;
	static const uint32_t HAS_2D            = 0x00000002;
	static const uint32_t HAS_3D            = 0x00000004;
	
public:
	WxStageCanvas(wxWindow* wnd, EditPanelImpl& stage,
		const std::shared_ptr<wxGLContext>& gl_ctx = nullptr, 
		const std::shared_ptr<gum::RenderContext>& gum_rc = nullptr,
		uint32_t flag = USE_CONTEXT_STACK | HAS_2D);
	virtual ~WxStageCanvas();

	const std::shared_ptr<wxGLContext>& GetGLContext() const { return m_gl_ctx; }
	const std::shared_ptr<gum::RenderContext>& GetGumRC() const { return m_gum_rc; }

	void SetDirty() { m_dirty = true; }

protected:
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDrawWhole() const;
	virtual void OnDrawSprites() const = 0;

	virtual void OnTimer() {}

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

private:
	void SetCurrentCanvas();

	void InitRender();
	void InitOthers();

	void BindRenderContext();

private:
	uint32_t m_flag;

	EditPanelImpl& m_stage;

	std::shared_ptr<wxGLContext>        m_gl_ctx = nullptr;
	std::shared_ptr<gum::RenderContext> m_gum_rc = nullptr;
	int m_ctx_idx_2d, m_ctx_idx_3d;

	wxTimer m_timer;

	bool m_dirty;

	DECLARE_EVENT_TABLE()

}; // WxStageCanvas

}
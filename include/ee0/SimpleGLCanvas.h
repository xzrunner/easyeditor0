#pragma once

#include <wx/glcanvas.h>

#include <memory>

namespace ee0
{

class SimpleGLCanvas : public wxGLCanvas
{
public:
	SimpleGLCanvas(wxWindow* parent);

protected:
	virtual void OnDraw() const = 0;

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

private:
	std::unique_ptr<wxGLContext> m_context;

	DECLARE_EVENT_TABLE()

}; // SimpleGLCanvas

}
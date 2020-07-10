#include "ee0/WxSimpleCanvas.h"

namespace ee0
{

BEGIN_EVENT_TABLE(WxSimpleCanvas, wxGLCanvas)
	EVT_SIZE(WxSimpleCanvas::OnSize)
	EVT_PAINT(WxSimpleCanvas::OnPaint)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

WxSimpleCanvas::WxSimpleCanvas(wxWindow* parent)
	: wxGLCanvas(parent, wxID_ANY, NULL)
	, m_context(std::make_unique<wxGLContext>(this))
{
}

void WxSimpleCanvas::OnSize(wxSizeEvent& event)
{
}

void WxSimpleCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);

	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	OnDraw();

	//glFlush();
	SwapBuffers();
}

}
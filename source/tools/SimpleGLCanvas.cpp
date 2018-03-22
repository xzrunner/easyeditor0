#include "ee0/SimpleGLCanvas.h"

namespace ee0
{

BEGIN_EVENT_TABLE(SimpleGLCanvas, wxGLCanvas)
	EVT_SIZE(SimpleGLCanvas::OnSize)
	EVT_PAINT(SimpleGLCanvas::OnPaint)
END_EVENT_TABLE()

static int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1, WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1, WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

SimpleGLCanvas::SimpleGLCanvas(wxWindow* parent)
	: wxGLCanvas(parent, wxID_ANY, NULL)
	, m_context(std::make_unique<wxGLContext>(this))
{
}

void SimpleGLCanvas::OnSize(wxSizeEvent& event)
{
}

void SimpleGLCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);

	glClearColor(1, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	OnDraw();

	glFlush();
	SwapBuffers();
}

}
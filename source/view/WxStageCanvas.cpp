#include "ee0/WxStageCanvas.h"
#include "ee0/EditPanelImpl.h"
#include "ee0/RenderContext.h"
#include "ee0/WindowContext.h"

#include <unirender/RenderContext.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <sprite2/SprTimer.h>
#include <gum/Sprite2.h>
#include <gum/Audio.h>
#include <gum/Model3.h>
#include <gum/RenderContext.h>
#include <gum/ShaderLab.h>
#include <gum/DTex.h>
#include <gum/Blackboard.h>

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/RenderContext.h>
#include <shaderlab/Shape2Shader.h>
#include <shaderlab/Shape3Shader.h>
#include <shaderlab/Sprite2Shader.h>
#include <shaderlab/Sprite3Shader.h>
#include <shaderlab/BlendShader.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/MaskShader.h>
#include <shaderlab/Model3Shader.h>

#include <guard/check.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxStageCanvas, wxGLCanvas)
	EVT_SIZE(WxStageCanvas::OnSize)
	EVT_PAINT(WxStageCanvas::OnPaint)
 	EVT_MOUSE_EVENTS(WxStageCanvas::OnMouse)
 	EVT_KEY_DOWN(WxStageCanvas::OnKeyDown)
	EVT_KEY_UP(WxStageCanvas::OnKeyUp)
	EVT_CHAR(WxStageCanvas::OnChar)
	EVT_KILL_FOCUS(WxStageCanvas::OnKillFocus)
END_EVENT_TABLE()

static const int GL_ATTRIB[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
								  WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
								  WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};

static const float FPS = 30;

WxStageCanvas::WxStageCanvas(wxWindow* wnd, EditPanelImpl& stage,
	                         const RenderContext* rc, 
	                         const WindowContext* wc,
	                         uint32_t flag)
	: wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB)
	, m_flag(flag)
	, m_stage(stage)
	, m_timer(this)
	, m_dirty(false)
{
	InitRender(rc);
	InitWindow(wc);
	InitOthers();

	Bind(wxEVT_TIMER, &WxStageCanvas::OnTimer, this, m_timer.GetId());

	m_timer.Start(1000 / FPS);
}

WxStageCanvas::~WxStageCanvas()
{
	m_timer.Stop();

	SetCurrentCanvas();

	//if (m_flag & HAS_2D) 
	//{
	//	auto& wc_mgr = pt2::Blackboard::Instance()->GetWndCtxMgr();
	//	wc_mgr.Pop();

	//	auto ctx = wc_mgr.Top();
	//	if (ctx) {
	//		m_rc.gum_rc->OnSize(ctx->GetScreenWidth(), ctx->GetScreenHeight());
	//	}
	//}
	//if (m_flag & HAS_3D) {
	//	pt3::Blackboard::Instance()->GetWndCtxMgr().Pop();
	//}

	gum::Blackboard::Instance()->SetRenderContext(nullptr);
	if (m_new_rc) {
		m_rc.gum_rc->Unbind();
	}
}

wxGLCanvas* WxStageCanvas::CreateWxGLCanvas(wxWindow* wnd)
{
	return new wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB);
}

void WxStageCanvas::CreateRenderContext(RenderContext& rc, wxGLCanvas* canvas)
{
	rc.gl_ctx = std::make_shared<wxGLContext>(canvas);
	canvas->SetCurrent(*rc.gl_ctx);

	rc.gum_rc = std::make_shared<gum::RenderContext>();
	auto& sl_rc = rc.gum_rc->GetSlRc();
	auto& shader_mgr = sl_rc.GetShaderMgr();

	shader_mgr.CreateShader(sl::SHAPE2, new sl::Shape2Shader(sl_rc));
	shader_mgr.CreateShader(sl::SHAPE3, new sl::Shape3Shader(sl_rc));
	shader_mgr.CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	shader_mgr.CreateShader(sl::SPRITE3, new sl::Sprite3Shader(sl_rc));
	shader_mgr.CreateShader(sl::BLEND, new sl::BlendShader(sl_rc));
	shader_mgr.CreateShader(sl::FILTER, new sl::FilterShader(sl_rc));
	shader_mgr.CreateShader(sl::MASK, new sl::MaskShader(sl_rc));
	shader_mgr.CreateShader(sl::MODEL3, new sl::Model3Shader(sl_rc));
}

void WxStageCanvas::CreateWindowContext(WindowContext& wc, bool has2d, bool has3d)
{
	if (has2d)
	{
		wc.wc2 = std::make_shared<pt2::WindowContext>();
		wc.wc2->Bind();
		pt2::Blackboard::Instance()->SetWindowContext(wc.wc2);
	}
	if (has3d)
	{
		wc.wc3 = std::make_shared<pt3::WindowContext>();
		wc.wc3->Bind();
		pt3::Blackboard::Instance()->SetWindowContext(wc.wc3);
	}
}

void WxStageCanvas::OnDrawWhole() const
{
	m_rc.gum_rc->GetUrRc().Clear(0x88888888);

	OnDrawSprites();
}

void WxStageCanvas::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();
	int w = size.GetWidth();
	int h = size.GetHeight();
	if (w != 0 && h != 0) 
	{
		SetCurrentCanvas();
		OnSize(w, h);
		m_rc.gum_rc->OnSize(w, h);
	}
}

void WxStageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	OnDrawWhole();
	m_dirty = false;

	m_rc.gum_rc->GetSlRc().GetShaderMgr().FlushShader();

	glFlush();
	SwapBuffers();

	gum::ShaderLab::Instance()->Update(1 / 30.0f);

//	wxPaintDC dc(this);
//	OnDrawDC();
}

void WxStageCanvas::OnMouse(wxMouseEvent& event)
{
	SetCurrentCanvas();

	m_stage.OnMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void WxStageCanvas::OnKeyDown(wxKeyEvent& event)
{
	SetCurrentCanvas();

	m_stage.OnKeyDown(event);
}

void WxStageCanvas::OnKeyUp(wxKeyEvent& event)
{
	SetCurrentCanvas();

	m_stage.OnKeyUp(event);
}

void WxStageCanvas::OnChar(wxKeyEvent& event)
{
	SetCurrentCanvas();

	m_stage.OnChar(event);
}

void WxStageCanvas::OnTimer(wxTimerEvent& event)
{
//	SetCurrentCanvas();

	OnTimer();

	float dt = 0.166f;
	clock_t curr_time = clock();
	static clock_t last_time = 0;
	if (last_time != 0) {
		dt = (float)(curr_time - last_time) / CLOCKS_PER_SEC;
	}
	last_time = curr_time;

	if (s2::SprTimer::Instance()->Update(dt)) {
		m_dirty = true;
	}

	gum::DTex::Instance()->Flush();

	// todo update
	//bool dirty = m_stage.Update();
	//if (dirty) {
	//	m_dirty = dirty;
	//}

	if (m_dirty) {
		Refresh();
	}
}

void WxStageCanvas::OnKillFocus(wxFocusEvent& event)
{
	SetCurrentCanvas();

//	m_stage.GetKeyState().Reset();
}

void WxStageCanvas::SetCurrentCanvas()
{
	BindRenderContext();

	if ((m_flag & HAS_2D) && m_wc.wc2) {
		m_wc.wc2->Bind();
		pt2::Blackboard::Instance()->SetWindowContext(m_wc.wc2);
	}
	if ((m_flag & HAS_3D) && m_wc.wc3) {
		m_wc.wc3->Bind();
		pt3::Blackboard::Instance()->SetWindowContext(m_wc.wc3);
	}
}

void WxStageCanvas::InitRender(const RenderContext* rc)
{
	if (rc) {
		m_rc = *rc;
		m_new_rc = false;
	} else {
		CreateRenderContext(m_rc, this);
		SetCurrentCanvas();
		m_new_rc = true;
	}
}

void WxStageCanvas::InitWindow(const WindowContext* wc)
{
	if (wc) {
		m_wc = *wc;
	} else {
		CreateWindowContext(m_wc, m_flag & HAS_2D, m_flag & HAS_3D);
	}
}

void WxStageCanvas::InitOthers()
{
	static bool inited = false;
	if (inited) {
		return;
	}

	s2::SprTimer::Instance()->Init();

	gum::Model3::Instance();
	gum::Sprite2::Instance()->Init();
	gum::Audio::Instance()->Initialize(nullptr, nullptr);
	//DTex::Init();
	//GTxt::Init();

	inited = true;
}

void WxStageCanvas::BindRenderContext()
{
	SetCurrent(*m_rc.gl_ctx);

	gum::Blackboard::Instance()->SetRenderContext(m_rc.gum_rc);
	m_rc.gum_rc->Bind();
}

}
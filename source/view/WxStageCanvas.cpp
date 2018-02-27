#include "ee0/WxStageCanvas.h"
#include "ee0/EditPanelImpl.h"

#include <unirender/RenderContext.h>
#include <painting2/RenderCtxStack.h>
#include <sprite2/SprTimer.h>
#include <node3/RenderCtxStack.h>
#include <gum/Sprite2.h>
#include <gum/Audio.h>
#include <gum/Model3.h>
#include <gum/RenderContext.h>
#include <gum/ShaderLab.h>
#include <gum/DTex.h>
#include <gum/Blackboard.h>

#include <shaderlab/ShaderMgr.h>
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
	                         const std::shared_ptr<wxGLContext>& gl_ctx, 
		                     const std::shared_ptr<gum::RenderContext>& gum_rc, 
	                         uint32_t flag)
	: wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB)
	, m_flag(flag)
	, m_stage(stage)
	, m_gl_ctx()
	, m_ctx_idx_2d(-1)
	, m_ctx_idx_3d(-1)
	, m_timer(this)
	, m_dirty(false)
{
	if (gl_ctx) 
	{
		m_gl_ctx = gl_ctx;
		GD_ASSERT(gum_rc, "null gum_rc");
		m_gum_rc = gum_rc;
	} 
	else 
	{
		m_gl_ctx = std::make_shared<wxGLContext>(this);
		SetCurrentCanvas();
		InitRender();
	}
	InitOthers();

	Bind(wxEVT_TIMER, &WxStageCanvas::OnTimer, this, m_timer.GetId());

	if (m_flag & USE_CONTEXT_STACK)
	{
		if (m_flag & HAS_2D) {
			m_ctx_idx_2d = pt2::RenderCtxStack::Instance()->Push(pt2::RenderContext());
		}
		if (m_flag & HAS_3D) {
			m_ctx_idx_3d = n3::RenderCtxStack::Instance()->Push(n3::RenderContext());
		}
	}

	m_timer.Start(1000 / FPS);
}

WxStageCanvas::~WxStageCanvas()
{
	m_timer.Stop();

	if (m_flag & USE_CONTEXT_STACK)
	{
		if (m_flag & HAS_2D) {
			pt2::RenderCtxStack::Instance()->Pop();

			auto ctx = pt2::RenderCtxStack::Instance()->Top();
			if (ctx) {
				m_gum_rc->OnSize(ctx->GetScreenWidth(), ctx->GetScreenHeight());
			}
		}
		if (m_flag & HAS_3D) {
			n3::RenderCtxStack::Instance()->Pop();
		}
	}
}

void WxStageCanvas::OnDrawWhole() const
{
	m_gum_rc->GetContext()->Clear(0x88888888);

	OnDrawSprites();
}

void WxStageCanvas::OnSize(wxSizeEvent& event)
{
	SetCurrentCanvas();

	wxSize size = event.GetSize();
	int w = size.GetWidth();
	int h = size.GetHeight();

	OnSize(w, h);

	m_gum_rc->OnSize(w, h);
}

void WxStageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering 
	// context context current
	SetCurrentCanvas();

	OnDrawWhole();
	m_dirty = false;

	m_gum_rc->GetShaderMgr()->FlushShader();

	glFlush();
	SwapBuffers();

	gum::ShaderLab::Instance()->Update(1 / 30.0f);

//	wxPaintDC dc(this);
//	OnDrawDC();
}

void WxStageCanvas::OnMouse(wxMouseEvent& event)
{
	m_stage.OnMouse(event);

	// The handler of this event should normally call event.Skip() to allow the default processing 
	// to take place as otherwise the window under mouse wouldn't get the focus.
	if (event.LeftDown()) {
		event.Skip();
	}
}

void WxStageCanvas::OnKeyDown(wxKeyEvent& event)
{
	m_stage.OnKeyDown(event);
}

void WxStageCanvas::OnKeyUp(wxKeyEvent& event)
{
	m_stage.OnKeyUp(event);
}

void WxStageCanvas::OnChar(wxKeyEvent& event)
{
	m_stage.OnChar(event);
}

void WxStageCanvas::OnTimer(wxTimerEvent& event)
{
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
//	m_stage.GetKeyState().Reset();
}

void WxStageCanvas::SetCurrentCanvas()
{
	BindRenderContext();

	if (m_flag & HAS_2D) {
		pt2::RenderCtxStack::Instance()->Bind(m_ctx_idx_2d);
	}
	if (m_flag & HAS_3D) {
		n3::RenderCtxStack::Instance()->Bind(m_ctx_idx_3d);
	}
}

void WxStageCanvas::InitRender()
{
	m_gum_rc = std::make_shared<gum::RenderContext>();
	auto& shader_mgr = *m_gum_rc->GetShaderMgr();

	shader_mgr.CreateShader(sl::SHAPE2, new sl::Shape2Shader(shader_mgr));
	shader_mgr.CreateShader(sl::SHAPE3, new sl::Shape3Shader(shader_mgr));
	shader_mgr.CreateShader(sl::SPRITE2, new sl::Sprite2Shader(shader_mgr));
	shader_mgr.CreateShader(sl::SPRITE3, new sl::Sprite3Shader(shader_mgr));
	shader_mgr.CreateShader(sl::BLEND, new sl::BlendShader(shader_mgr));
	shader_mgr.CreateShader(sl::FILTER, new sl::FilterShader(shader_mgr));
	shader_mgr.CreateShader(sl::MASK, new sl::MaskShader(shader_mgr));
	shader_mgr.CreateShader(sl::MODEL3, new sl::Model3Shader(shader_mgr));
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
	SetCurrent(*m_gl_ctx);

	if (m_gum_rc) {
		gum::Blackboard::Instance()->SetRenderContext(m_gum_rc);
		m_gum_rc->Bind();
	}
}

}
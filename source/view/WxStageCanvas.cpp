#include "ee0/WxStageCanvas.h"
#include "ee0/EditPanelImpl.h"
#include "ee0/RenderContext.h"
#include "ee0/WindowContext.h"
#include "ee0/EditOP.h"
#include "ee0/ConfigFile.h"

#include <guard/check.h>
#include <unirender/RenderContext.h>
#include <painting2/Shader.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting2/RenderSystem.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <facade/Blackboard.h>
#include <facade/RenderContext.h>
#include <facade/Facade.h>
#include <facade/DTex.h>
#include <facade/EasyGUI.h>
#include <rendergraph/RenderMgr.h>
#include <rendergraph/SpriteRenderer.h>

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
	                         const pt0::CameraPtr& camera,
	                         const RenderContext* rc,
	                         const WindowContext* wc,
	                         uint32_t flag)
	: wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
	, m_camera(camera)
	, m_flag(flag)
	, m_stage(stage)
	, m_timer(this)
	, m_last_time(0)
	, m_dirty(false)
{
	InitRender(rc);

	SetCurrentCanvas();

	InitWindow(wc);

	EnableInitiativeUpdate(true);

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
	//		m_rc.facade_rc->OnSize(ctx->GetScreenWidth(), ctx->GetScreenHeight());
	//	}
	//}
	//if (m_flag & HAS_3D) {
	//	pt3::Blackboard::Instance()->GetWndCtxMgr().Pop();
	//}

	facade::Blackboard::Instance()->SetRenderContext(nullptr);
	if (m_new_rc) {
		m_rc.facade_rc->Unbind();
	}
}

void WxStageCanvas::EnableInitiativeUpdate(bool enable)
{
	if (enable) {
		Bind(wxEVT_TIMER, &WxStageCanvas::OnTimer, this, m_timer.GetId());
	} else {
		Unbind(wxEVT_TIMER, &WxStageCanvas::OnTimer, this, m_timer.GetId());
	}
}

// fixme: changed shader's mat
void WxStageCanvas::PrepareDrawGui(float w, float h) const
{
	ee0::RenderContext::Reset2D();

	auto sr = std::static_pointer_cast<rg::SpriteRenderer>(
		rg::RenderMgr::Instance()->SetRenderer(rg::RenderType::SPRITE)
	);
	auto& palette = sr->GetPalette();

	auto shader = sr->GetShader();
	shader->Use();

	auto pt2_shader = std::dynamic_pointer_cast<pt2::Shader>(shader);
	assert(pt2_shader);
	pt2_shader->UpdateViewMat(sm::vec2(0, 0), 1.0f);

	pt2_shader->UpdateProjMat(w, h);

	shader->UpdateModelMat(sm::mat4());

	auto tex_id = facade::DTex::Instance()->GetSymCacheTexID();
	m_rc.facade_rc->GetUrRc().BindTexture(tex_id, 0);
}

wxGLCanvas* WxStageCanvas::CreateWxGLCanvas(wxWindow* wnd)
{
	return new wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB);
}

void WxStageCanvas::CreateRenderContext(RenderContext& rc, wxGLCanvas* canvas)
{
	rc.gl_ctx = std::make_shared<wxGLContext>(canvas);
	canvas->SetCurrent(*rc.gl_ctx);

	rc.facade_rc = std::make_shared<facade::RenderContext>();
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
	OnDrawSprites();
	OnDrawGUI();

	if (ConfigFile::Instance()->GetDebugDraw()) {
		DebugDraw();
	}
}

void WxStageCanvas::InitGui()
{
	facade::EasyGUI::Instance();
	auto& wc = const_cast<ee0::WindowContext&>(GetWidnowContext());
	wc.egui = std::make_shared<egui::Context>();
	egui::style_colors_dark(wc.egui->style);
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
//		m_rc.facade_rc->OnSize(w, h);
	}
}

void WxStageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering
	// context context current
	SetCurrentCanvas();

	m_camera->Bind();

	OnDrawWhole();
	m_dirty = false;

	rg::RenderMgr::Instance()->Flush();

	glFlush();
	SwapBuffers();

	//gum::ShaderLab::Instance()->Update(1 / 30.0f);

//	wxPaintDC dc(this);
//	OnDrawDC();
}

void WxStageCanvas::OnMouse(wxMouseEvent& event)
{
	SetCurrentCanvas();

	m_stage.OnMouse(event);

	OnMouseImpl(event);

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

	OnKeyDownImpl(event);
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
	// for m_tasks
	SetCurrentCanvas();

	for (auto& task : m_tasks) {
		task();
	}
	m_tasks.clear();

	OnTimer();

	float dt = 0.166f;
	clock_t curr_time = clock();
	if (m_last_time != 0) {
		dt = (float)(curr_time - m_last_time) / CLOCKS_PER_SEC;
	}
	m_last_time = curr_time;

	auto facade = facade::Facade::Instance();
	facade->Update(dt);
	facade->Flush(true);

	if (auto& op = m_stage.GetEditOP()) {
		op->Update(dt);
	}

	if (OnUpdate()) {
		m_dirty = true;
	}

	if (m_dirty) {
		Refresh();
	}
}

void WxStageCanvas::OnKillFocus(wxFocusEvent& event)
{
	SetCurrentCanvas();

//	m_stage.GetKeyState().Reset();
}

void WxStageCanvas::DebugDraw() const
{
	facade::DTex::Instance()->DebugDraw();
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

void WxStageCanvas::BindRenderContext()
{
	SetCurrent(*m_rc.gl_ctx);

	facade::Blackboard::Instance()->SetRenderContext(m_rc.facade_rc);
	m_rc.facade_rc->Bind();
}

}
#include "ee0/WxStageCanvas.h"
#include "ee0/EditPanelImpl.h"
#include "ee0/RenderContext.h"
#include "ee0/WindowContext.h"
#include "ee0/EditOP.h"
#include "ee0/ConfigFile.h"

#include <guard/check.h>
#include <unirender2/Factory.h>
#include <unirender2/ShaderProgram.h>
#include <unirender2/ClearState.h>
#include <painting0/ModelMatUpdater.h>
#include <painting2/Shader.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting2/RenderSystem.h>
#include <painting2/ViewMatUpdater.h>
#include <painting2/ProjectMatUpdater.h>
#include <painting2/OrthoCamera.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <facade/Blackboard.h>
#include <facade/RenderContext.h>
#include <facade/Facade.h>
#include <facade/DTex.h>
#include <facade/EasyGUI.h>
#include <renderpipeline/RenderMgr.h>
#include <renderpipeline/SpriteRenderer.h>

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

WxStageCanvas::WxStageCanvas(const ur2::Device& dev,
                             wxWindow* wnd, EditPanelImpl& stage,
	                         const pt0::CameraPtr& camera,
	                         const RenderContext* rc,
	                         const WindowContext* wc,
	                         uint32_t flag)
	: wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
    , m_dev(dev)
	, m_camera(camera)
	, m_flag(flag)
	, m_stage(stage)
	, m_timer(this)
	, m_last_time(0)
	, m_dirty(false)
{
	InitRender(dev, rc);

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
		//m_rc.facade_rc->Unbind();
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
void WxStageCanvas::PrepareDrawGui(const ur2::Device& dev, float w, float h) const
{
	ee0::RenderContext::Reset2D();

	auto rd = std::static_pointer_cast<rp::SpriteRenderer>(
		rp::RenderMgr::Instance()->SetRenderer(dev, *m_rc.ur_ctx, rp::RenderType::SPRITE)
	);
	auto& palette = rd->GetPalette();

	auto shader = rd->GetAllShaders()[0];
//	shader->Bind();

    auto model_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt0::ModelMatUpdater>());
    if (model_updater) {
        std::static_pointer_cast<pt0::ModelMatUpdater>(model_updater)->Update(sm::mat4());
    }
    auto view_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt2::ViewMatUpdater>());
    if (view_updater) {
        std::static_pointer_cast<pt2::ViewMatUpdater>(view_updater)->Update(sm::vec2(0, 0), 1.0f);
    }
    auto proj_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt2::ProjectMatUpdater>());
    if (proj_updater) {
        std::static_pointer_cast<pt2::ProjectMatUpdater>(proj_updater)->Update(w, h);
    }

 //   // todo
	//auto tex_id = facade::DTex::Instance()->GetSymCacheTexID();
	//m_rc.facade_rc->GetUrRc().BindTexture(tex_id, 0);
}

wxGLCanvas* WxStageCanvas::CreateWxGLCanvas(wxWindow* wnd)
{
	return new wxGLCanvas(wnd, wxID_ANY, GL_ATTRIB);
}

std::shared_ptr<ur2::Device>
WxStageCanvas::CreateRenderContext(const ur2::Device* dev, RenderContext& rc, wxGLCanvas* canvas)
{
    rc.gl_ctx = std::make_shared<wxGLContext>(canvas);
    canvas->SetCurrent(*rc.gl_ctx);

    std::shared_ptr<ur2::Device> ret = nullptr;
    if (!dev) {
        ret = ur2::CreateDeviceGL();
        dev = ret.get();
    }

    rc.ur_ctx = ur2::CreateContextGL(*dev);

    //rc.facade_rc = std::make_shared<facade::RenderContext>();

    return ret;
}

void WxStageCanvas::CreateWindowContext(WindowContext& wc, bool has2d, bool has3d)
{
	//if (has2d)
	//{
	//	wc.wc2 = std::make_shared<pt2::WindowContext>();
	//	wc.wc2->Bind();
	//	pt2::Blackboard::Instance()->SetWindowContext(wc.wc2);
	//}
	//if (has3d)
	//{
	//	wc.wc3 = std::make_shared<pt3::WindowContext>();
	//	wc.wc3->Bind();
	//	pt3::Blackboard::Instance()->SetWindowContext(wc.wc3);
	//}
}

void WxStageCanvas::OnDrawWhole() const
{
    //if (GetWidnowContext().wc2) {
    //    rp::RenderMgr::Instance()->BindWndCtx2D(
    //        std::const_pointer_cast<pt2::WindowContext>(GetWidnowContext().wc2)
    //    );
    //}
    //if (GetWidnowContext().wc3) {
    //    rp::RenderMgr::Instance()->BindWndCtx3D(
    //        std::const_pointer_cast<pt3::WindowContext>(GetWidnowContext().wc3)
    //    );
    //}

    auto spr_rd = rp::RenderMgr::Instance()->GetRenderer(rp::RenderType::SPRITE);
    if (spr_rd)
    {
        auto& shaders = spr_rd->GetAllShaders();
        assert(shaders.size() == 1);
        auto& shader = shaders[0];

        auto model_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt0::ModelMatUpdater>());
        if (model_updater) {
            std::static_pointer_cast<pt0::ModelMatUpdater>(model_updater)->Update(sm::mat4());
        }
        auto view_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt2::ViewMatUpdater>());
        if (view_updater) {
            sm::vec2 offset(0, 0);
            float scale = 1.0f;
            if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
                auto o_cam = std::static_pointer_cast<pt2::OrthoCamera>(m_camera);
                offset = -o_cam->GetPosition();
                scale = 1.0f / o_cam->GetScale();
            }
            std::static_pointer_cast<pt2::ViewMatUpdater>(view_updater)->Update(offset, scale);
        }
        auto proj_updater = shader->QueryUniformUpdater(ur2::GetUpdaterTypeID<pt2::ProjectMatUpdater>());
        if (proj_updater) {
            std::static_pointer_cast<pt2::ProjectMatUpdater>(proj_updater)->Update(m_screen_sz.x, m_screen_sz.y);
        }
    }

    ur2::ClearState clear;
    clear.buffers = ur2::ClearBuffers::ColorBuffer;
    clear.color.FromRGBA(m_bg_color);
    GetRenderContext().ur_ctx->Clear(clear);

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
        m_rc.ur_ctx->SetViewport(0, 0, w, h);
        m_camera->OnSize(w, h);
        m_screen_sz.Set(w, h);
	}
}

void WxStageCanvas::OnPaint(wxPaintEvent& event)
{
	// Makes the OpenGL state that is represented by the OpenGL rendering
	// context context current
	SetCurrentCanvas();

    m_rc.ur_ctx->SetViewport(0, 0, m_screen_sz.x, m_screen_sz.y);

	m_camera->Bind();

	OnDrawWhole();
	m_dirty = false;

	rp::RenderMgr::Instance()->Flush(m_dev, *m_rc.ur_ctx);

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
	facade->Flush(*GetRenderContext().ur_ctx, true);

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

	//if ((m_flag & HAS_2D) && m_wc.wc2) {
	//	m_wc.wc2->Bind();
	//	pt2::Blackboard::Instance()->SetWindowContext(m_wc.wc2);
	//}
	//if ((m_flag & HAS_3D) && m_wc.wc3) {
	//	m_wc.wc3->Bind();
	//	pt3::Blackboard::Instance()->SetWindowContext(m_wc.wc3);
	//}
}

void WxStageCanvas::InitRender(const ur2::Device& dev, const RenderContext* rc)
{
	if (rc) {
		m_rc = *rc;
		m_new_rc = false;
	} else {
		CreateRenderContext(&dev, m_rc, this);
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

	//facade::Blackboard::Instance()->SetRenderContext(m_rc.facade_rc);
	//m_rc.facade_rc->Bind();
}

}
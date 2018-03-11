#pragma once

#include <wx/frame.h>

#include <memory>

namespace ee0
{

class Application;

class WxFrame : public wxFrame
{
public:
	WxFrame(const std::string& title, bool maxmize = false,
		const wxSize& size = wxSize(800, 600));

	void SetApp(const std::shared_ptr<Application>& app) {
		m_app = app;
	}

protected:
	virtual void OnNew(wxCommandEvent& event);
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSave(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);
	virtual void OnClear(wxCommandEvent& event);

	virtual void OnSettings(wxCommandEvent& event);

private:
	void InitMenuBar();
	void InitStatueBar();

	wxMenu* InitFileBar();
	wxMenu* InitSettingsBar();

private:
	enum
	{
		ID_SETTINGS = 1000,
	};

protected:
	std::shared_ptr<Application> m_app = nullptr;

	DECLARE_EVENT_TABLE()

}; // WxFrame

}
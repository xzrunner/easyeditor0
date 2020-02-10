#include "ee0/WxFrame.h"
#include "ee0/Application.h"
#include "ee0/AssetsMap.h"

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxFrame, wxFrame)
	EVT_MENU(wxID_NEW,          WxFrame::OnNew)
	EVT_MENU(wxID_OPEN,         WxFrame::OnOpen)
	EVT_MENU(wxID_SAVE,         WxFrame::OnSave)
	EVT_MENU(wxID_SAVEAS,       WxFrame::OnSaveAs)
	EVT_MENU(wxID_CLEAR,        WxFrame::OnClear)
	EVT_MENU(ID_SETTINGS,       WxFrame::OnSettings)
    EVT_MENU(ID_ADD_ASSETS_DIR, WxFrame::OnAddAssetsDir)
END_EVENT_TABLE()

WxFrame::WxFrame(const std::string& title, const wxSize& size, long style)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, style)
{
	InitMenuBar();
	InitStatueBar();
}

void WxFrame::OnNew(wxCommandEvent& event)
{
	m_app->Clear();
}

void WxFrame::OnOpen(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString,
		"*.json", wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK) {
		m_app->LoadFromFile(dlg.GetPath().ToStdString());
	}
}

void WxFrame::OnSave(wxCommandEvent& event)
{
	wxMessageDialog dlg(NULL, wxT("Are you sure to save?"),
		wxT("Question"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if (dlg.ShowModal() == wxID_YES) {
		m_app->StoreToFile("");
	}
}

void WxFrame::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString,
		"*.json", wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK) {
		m_app->StoreToFile(dlg.GetPath().ToStdString());
	}
}

void WxFrame::OnClear(wxCommandEvent& event)
{
	m_app->Clear();
}

void WxFrame::OnSettings(wxCommandEvent& event)
{
}

void WxFrame::OnAddAssetsDir(wxCommandEvent& event)
{
    wxDirDialog dlg(NULL, "Assets", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK) {
        AssetsMap::Instance()->LoadDirWithUnity(dlg.GetPath().ToStdString());
    }
}

void WxFrame::InitMenuBar()
{
	wxMenuBar* menu_bar = new wxMenuBar;
	menu_bar->Append(InitFileBar(), "&File");
	menu_bar->Append(InitSettingsBar(), "&Settings");
	SetMenuBar(menu_bar);
}

void WxFrame::InitStatueBar()
{
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
}

wxMenu* WxFrame::InitFileBar()
{
	//for (int i = 0; i < RecentFilesMenu::CAPACITY; ++i) {
	//	Bind(wxEVT_COMMAND_MENU_SELECTED, &WxFrame::OnOpenRecentFile, this, ID_RECENT_FILENAME + i);
	//}

	wxMenu* menu = new wxMenu;
	menu->Append(wxID_NEW, wxT("&New...\tCtrl+N"), wxT("Create a project"));
	menu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	menu->AppendSeparator();
	menu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	menu->Append(wxID_SAVEAS, wxT("&Save as...\tCtrl+Shift+S"), wxT("Save to a new file"));
	menu->AppendSeparator();
	menu->Append(wxID_CLEAR, wxT("Clear"), wxT("Clear the project"));
	menu->AppendSeparator();
	//	menu->AppendSubMenu(m_recent_menu->GetMenu(), wxT("Recent Files"));
	//menu->AppendSeparator();
	//menu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return menu;
}

wxMenu* WxFrame::InitSettingsBar()
{
	wxMenu* menu = new wxMenu;
	menu->Append(ID_SETTINGS, wxT("Base"), wxT("Base"));
    menu->Append(ID_ADD_ASSETS_DIR, wxT("Add assets dir..."), wxT("Add assets dir"));
	return menu;
}

}
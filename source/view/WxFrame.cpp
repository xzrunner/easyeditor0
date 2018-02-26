#include "ee0/WxFrame.h"
#include "ee0/Application.h"

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>

namespace ee0
{

BEGIN_EVENT_TABLE(WxFrame, wxFrame)
	EVT_MENU(wxID_NEW, WxFrame::OnNew)
	EVT_MENU(wxID_OPEN, WxFrame::OnOpen)
	EVT_MENU(wxID_SAVE, WxFrame::OnSave)
	EVT_MENU(wxID_SAVEAS, WxFrame::OnSaveAs)
END_EVENT_TABLE()

WxFrame::WxFrame(const std::string& title, bool maxmize, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, wxDEFAULT_FRAME_STYLE | (wxMAXIMIZE * maxmize))
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

void WxFrame::InitMenuBar()
{
	wxMenuBar* menu_bar = new wxMenuBar;
	menu_bar->Append(InitFileBar(), "&File");
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
	menu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Create a project"));
	menu->Append(wxID_OPEN, wxT("&Open...\tCtrl+O"), wxT("Open a project"));
	menu->AppendSeparator();
	menu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Save the project"));
	menu->Append(wxID_SAVEAS, wxT("&Save as...\tF11"), wxT("Save to a new file"));
	menu->AppendSeparator();
//	menu->AppendSubMenu(m_recent_menu->GetMenu(), wxT("Recent Files"));
	//menu->AppendSeparator();
	//menu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quit"));
	return menu;
}

}
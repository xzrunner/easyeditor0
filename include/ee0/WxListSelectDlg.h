#pragma once

#include <wx/dialog.h>
#include <wx/treectrl.h>

#include <map>

namespace ee0
{

class WxListSelectDlg : public wxDialog
{
public:
	WxListSelectDlg(wxWindow* parent, const std::string& title,
		const std::vector<std::pair<std::string, wxTreeItemData*>>& list,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	wxTreeItemData* GetSelected() const;

private:
	void InitLayout(const std::vector<std::pair<std::string, wxTreeItemData*>>& list);

	void OnSelChanged(wxTreeEvent& event);
	void OnDoubleClick(wxTreeEvent& event);

private:
	wxTreeCtrl* m_tree;

}; // WxListSelectDlg

}
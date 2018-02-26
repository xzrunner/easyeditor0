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
		const std::vector<std::pair<uint32_t, std::string>>& list,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	uint32_t GetSelectedID() const;

private:
	void InitLayout(const std::vector<std::pair<uint32_t, std::string>>& list);
	
	void OnSelChanged(wxTreeEvent& event);
	void OnDoubleClick(wxTreeEvent& event);

private:
	wxTreeCtrl* m_tree;

	std::map<std::string, uint32_t> m_name2id;

}; // WxListSelectDlg

}
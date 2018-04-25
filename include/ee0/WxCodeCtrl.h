#pragma once

#include "ee0/prefs.h"

#include <wx/stc/stc.h>

namespace ee0
{

class WxCodeCtrl : public wxStyledTextCtrl
{
public:
	WxCodeCtrl(wxWindow* parent, const std::string& name);
	virtual ~WxCodeCtrl() {}

	const std::string& getName() const;

	bool InitializePrefs(const std::string& filename);

protected:
	//! language/lexer
	wxString DeterminePrefs (const wxString& filename);

private:
	std::string m_name;

	// lanugage properties
	const LanguageInfo* m_language;

	// margin variables
	int m_LineNrID;
	int m_LineNrMargin;
	int m_FoldingID;
	int m_FoldingMargin;
	int m_DividerID;

}; // WxCodeCtrl

}
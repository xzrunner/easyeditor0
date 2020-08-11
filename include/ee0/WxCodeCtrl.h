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

	bool InitializePrefs(const std::string& filename);

protected:
	//! language/lexer
	wxString DeterminePrefs (const wxString& filename);

private:
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
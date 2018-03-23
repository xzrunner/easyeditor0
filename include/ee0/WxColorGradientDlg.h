#pragma once

#include "ee0/SimpleGLCanvas.h"

#include <painting2/GradientColor.h>

#include <wx/dialog.h>

class wxColourPickerEvent;
class wxColourPickerCtrl;

namespace ee0
{

class WxColorGradientDlg : public wxDialog
{
public:
	WxColorGradientDlg(wxWindow* parent, const pt2::GradientColor& col);

	pt2::GradientColor GetColor() const;

private:
	class ColorPreview : public wxPanel
	{
	public:
		ColorPreview(wxWindow* parent, wxSize size, pt2::GradientColor& col);

	private:
		void OnSize(wxSizeEvent& event);

	private:
		class Canvas : public SimpleGLCanvas
		{
		public:
			Canvas(wxWindow* parent, pt2::GradientColor& col);

		protected:
			virtual void OnDraw() const override;

		private:
			void DrawItem(const pt2::Color& col, float pos) const;

		private:
			pt2::GradientColor& m_col;

		}; // Canvas

	private:
		std::unique_ptr<Canvas> m_canvas = nullptr;

		DECLARE_EVENT_TABLE()

	}; // ColorPreview

private:
	void InitColor();
	void InitLayout();

	void CommandEventHandler(wxCommandEvent& event);
	void SpinEventHandler(wxSpinEvent& event);
	void ColourPickerEventHandler(wxColourPickerEvent& event);

private:
	pt2::GradientColor m_col;

	wxColourPickerCtrl *m_begin_col, *m_mid_col, *m_end_col;

	wxTextCtrl *m_begin_pos, *m_mid_pos, *m_end_pos;

	ColorPreview* m_preview;

	wxSpinCtrl* m_angle;

}; // WxColorGradientDlg

}
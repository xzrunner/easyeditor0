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

	class ColorSlider : public wxPanel
	{
	public:
		ColorSlider(wxWindow* parent, wxSize size, pt2::GradientColor& col);

	private:
		void OnPaint(wxPaintEvent& event);
		void OnMouse(wxMouseEvent& event);

		void SelectByPos(int x);
		
	private:
		static const int HALF_WIDTH = 5;

	private:
		pt2::GradientColor& m_col;

		int m_selected;

		DECLARE_EVENT_TABLE()

	}; // ColorSlider

private:
	void InitLayout();

	void SpinEventHandler(wxSpinEvent& event);

private:
	pt2::GradientColor m_col;

	ColorPreview* m_preview;
	ColorSlider* m_slider;

	wxSpinCtrl* m_angle;

}; // WxColorGradientDlg

}
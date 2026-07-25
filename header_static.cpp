#include "stdafx.h"
#include "header_static.h"

#include "libPPUI/PaintUtils.h"
#include "libPPUI/wtl-pp.h"

using PaintUtils::PaintSeparatorControl;

void HeaderStatic::OnPaint(CDCHandle dcDummy)
{
	PaintSeparatorControl(*this);
}

BOOL HeaderStatic::SubclassWindow(HWND hWnd)
{
	if (!CWindowImpl<HeaderStatic, CStatic>::SubclassWindow(hWnd)) {
		return FALSE;
	}

	HFONT f = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	m_cFont.Attach(f);

	LOGFONT lf;
	m_cFont.GetLogFont(lf);

	CLogFont clogFont(m_cFont);
	clogFont.MakeLarger(4);
	//clogFont.MakeLarger(2);
	clogFont.MakeBolder(2);

	m_cFont.DeleteObject();
	HFONT hf = clogFont.CreateFontIndirect();
	m_cFont.Attach(hf);

	return TRUE;
}

void HeaderStatic::PaintHeader()
{
	CPaintDC dc(m_hWnd);

	CRect rect;
	GetClientRect(&rect);

	CString strText;
	GetWindowText(strText);
	SetFont(m_cFont);

	DWORD dwStyle = GetStyle();
	if ((dwStyle & SS_CENTER) == SS_CENTER)
		dc.DrawText(strText, -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	else if ((dwStyle & SS_LEFT) == SS_LEFT)
	{
		rect.left += m_iLeftSpacing;
		dc.DrawText(strText, -1, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	}
	else // Right
	{
		rect.right -= m_iLeftSpacing;
		dc.DrawText(strText, -1, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	}
}

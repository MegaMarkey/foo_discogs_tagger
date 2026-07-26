#include "stdafx.h"
#include "foo_discogs.h"
#include "prompt_dialog.h"

LRESULT CPromptDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {

	SetIcon(g_discogs->icon);
	uSetDlgItemText(m_hWnd, IDC_PROMPT_LABEL, m_ask);
	fb2k::inMainThread([this] {
		m_dark.AddDialogWithControls(m_hWnd);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		::SetFocus(m_hWnd);
		});

	RECT mainRect;
	::GetWindowRect(GetParent().m_hWnd, &mainRect);

	int x = mainRect.left + 8;
	int y = mainRect.top + 170;
	::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);


	return TRUE;
}

LRESULT CPromptDialog::OnButtonOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	uGetDlgItemText(m_hWnd, IDC_PROMPT_EDIT, *mp_result);
	EndDialog(0);
	return TRUE;
}

LRESULT CPromptDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(0);
	return TRUE;
}

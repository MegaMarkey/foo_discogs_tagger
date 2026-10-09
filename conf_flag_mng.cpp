#include "stdafx.h"
#include "conf.h"
#include "conf_flag_mng.h"

//constructor

FlgMng::FlgMng(CConf* pconf, int ID) : m_pcfg(pconf), m_id(ID) {
	//..
}

//get flags

bool FlgMng::GetFlag(int flg) {

	int* varval = m_pcfg ? m_pcfg->id_to_ref_int(m_id) : nullptr;
	return varval && (*varval & flg);
}

//set flags

bool FlgMng::SetFlag(HWND hWnd, UINT uid, int flg, bool inv) {
	PFC_ASSERT(GetDlgItem(hWnd, uid));
	bool flgval = uButton_GetCheck(hWnd, uid);
	if (inv) {
		//ej. 'auto-load' vs 'skip-load'
		flgval = !flgval;
	}
	return SetFlag(flg, flgval);
}

bool FlgMng::SetFlag(int flg, bool flgval) {

	if (int* varval = m_pcfg ? m_pcfg->id_to_ref_int(m_id) : nullptr) {
		if (flgval) {
			*varval |= flg;
		}
		else {
			*varval &= ~flg;
		}
	}
	return flgval;
}

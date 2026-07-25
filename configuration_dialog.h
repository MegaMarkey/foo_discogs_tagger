#pragma once
#include "resource.h"

#include "libPPUI/CDialogResizeHelper.h"
#include "helpers/DarkMode.h"

#include "header_static.h"
#include "guids_discogger.h"
#include "foo_discogs.h"
#include "my_tabentry.h"

#define NUM_TABS 8

#define CONF_FIND_RELEASE_TAB        0
#define CONF_FIND_ADV_RELEASE_TAB    1
#define CONF_MATCHING_TAB            2
#define CONF_TAGGING_TAB             3
#define CONF_CACHING_TAB             4
#define CONF_ART_TAB                 5
#define CONF_UI_TAB                  6
#define CONF_OATH_TAB                7

//snapLeft, snapTop, snapRight, snapBottom
const CDialogResizeHelper::Param rz_params[] = {

	{IDC_GRP_STATIC_TRK_MATCHING, 0,0,1,0},
	{IDC_GRP_STATIC_AUTO_TRK_MATCHING, 0,0,1,0},

	{IDC_GRP_STATIC_CFG_WRITE_TAGS, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_ARTIST_NAMES, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_MULTIVALUES, 0,0,1,0},

	{IDC_GRP_STATIC_CFG_CACHE_PARSE, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_CACHE_MEM, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_CACHE_DISK, 0,0,1,0},

	{IDC_GRP_STATIC_CFG_ART_ALBUM, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_ART_ARTIST, 0,0,1,0},

	{IDC_GRP_STATIC_HISTORY, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_OTHER, 0,0,1,0},

	{IDC_GRP_STATIC_CFG_OATH_STEP_1, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_OATH_STEP_2, 0,0,1,0},
	{IDC_GRP_STATIC_CFG_OATH_STEP_3, 0,0,1,0},
};

class NOVTABLE my_threaded_process : public threaded_process {
public:
	bool run_modal(service_ptr_t<threaded_process_callback> p_callback, unsigned p_flags, HWND p_parent, const char* p_title, t_size p_title_len) override;
	bool run_modeless(service_ptr_t<threaded_process_callback> p_callback, unsigned p_flags, HWND p_parent, const char* p_title, t_size p_title_len) override;

	//! Decrements reference count; deletes the object if reference count reaches zero. This is normally not called directly but managed by service_ptr_t<> template. \n
	//! Implemented by service_impl_* classes.
	//! @returns New reference count. For debug purposes only, in certain conditions return values may be unreliable.
	int service_release() throw() override;
	//! Increments reference count. This is normally not called directly but managed by service_ptr_t<> template. \n
	//! Implemented by service_impl_* classes.
	//! @returns New reference count. For debug purposes only, in certain conditions return values may be unreliable.
	int service_add_ref() throw() override;
	//! Queries whether the object supports specific interface and retrieves a pointer to that interface. This is normally not called directly but managed by service_query_t<> function template. \n
	//! Checks the parameter against GUIDs of interfaces supported by this object, if the GUID is one of supported interfaces, p_out is set to service_base pointer that can be static_cast<>'ed to queried interface and the method returns true; otherwise the method returns false. \n
	//! Implemented by service_impl_* classes. \n
	//! Note that service_query() implementation semantics (but not usage semantics) changed in SDK for foobar2000 1.4; they used to be auto-implemented by each service interface (via FB2K_MAKE_SERVICE_INTERFACE macro); they're now implemented in service_impl_* instead. See SDK readme for more details. \n
	bool service_query(service_ptr& p_out, const GUID& p_guid) override;
};

class CConfigurationDialog : public MyCDialogImpl<CConfigurationDialog>, public CMessageFilter, public preferences_page_instance);
	void save_caching_dialog(HWND wnd, bool dlgbind);
	void save_art_dialog(HWND wnd, bool dlgbind);
	void save_ui_dialog(HWND wnd, bool dlgbind);
	void save_oauth_dialog(HWND wnd, bool dlgbind);

	bool cfg_searching_has_changed();
	bool cfg_searching_adv_has_changed();
	bool cfg_matching_has_changed();
	bool cfg_tagging_has_changed();
	bool cfg_caching_has_changed();
	bool cfg_art_has_changed();
	bool cfg_ui_has_changed();
	bool cfg_oauth_has_changed();

	void on_load_search_formatting(HWND wnd);
	void on_load_search_adv_formatting(HWND wnd);
	void on_load_match_formatting(HWND wnd);
	void on_delete_history(HWND wnd, size_t max, bool zap);
	void on_test_oauth(HWND wnd);
	void on_authorize_oauth(HWND wnd);
	void on_generate_oauth(HWND wnd);

	bool HasChanged();
	void OnChanged();
	void OnEditChange(UINT, int, CWindow) { OnChanged(); }

	const preferences_page_callback::ptr m_callback;

public:

	enum { IDD = IDD_DIALOG_CONF };

	t_uint32 get_state();
	void apply();
	void reset();

	virtual BOOL PreTranslateMessage(MSG* pMsg) override {
		return ::IsDialogMessage(m_hWnd, pMsg);
	}

#pragma warning( push )
#pragma warning( disable : 26454 )

	MY_BEGIN_MSG_MAP(CConfigurationDialog)
		CHAIN_MSG_MAP_MEMBER(m_resize_helper)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		NOTIFY_HANDLER(IDC_TAB_CFG, TCN_SELCHANGING, OnChangingTab)
		NOTIFY_HANDLER(IDC_TAB_CFG, TCN_SELCHANGE, OnChangeTab)
		COMMAND_ID_HANDLER(IDC_BTN_CONF_DEFAULTS, OnDefaults)
		MESSAGE_HANDLER(WM_CUSTOM_ANV_CHANGED, OnCustomAnvChanged)
#ifdef SIM_VA_MA_BETA
		MESSAGE_HANDLER(WM_CUSTOM_VA_AS_MA_RESET, OnCustomMsg_VA_as_MA_Reset)
#endif
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MY_END_MSG_MAP()

#pragma warning( pop )

	// constructor

	CConfigurationDialog(preferences_page_callback::ptr callback) :	m_callback(callback), m_resize_helper(rz_params) {

		m_hwndTokenEdit = nullptr;
		m_hwndSecretEdit = nullptr;
		m_hwndOAuthMsg = nullptr;

		if (g_discogs) {
			g_discogs->configuration_dialog = this;
			conf = CConf(CONF); 
			conf_edit = CConf(CONF);
		}
	}

	~CConfigurationDialog();

	void InitTabs();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChangingTab(WORD /*wNotifyCode*/, LPNMHDR /*lParam*/, BOOL& /*bHandled*/); 
	LRESULT OnChangeTab(WORD /*wNotifyCode*/, LPNMHDR /*lParam*/, BOOL& /*bHandled*/); 
	LRESULT OnDefaults(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCustomAnvChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
#ifdef SIM_VA_MA_BETA
	LRESULT OnCustomMsg_VA_as_MA_Reset(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
#endif
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	HWND m_hwndTokenEdit;
	HWND m_hwndSecretEdit;
	HWND m_hwndOAuthMsg;

	void show_tab(unsigned int itab);
	void show_oauth_msg(pfc::string8 msg, bool iserror);

	void enable(bool v) override {}

private:

	my_threaded_process m_tp;

	pfc::array_t<tab_entry> tab_table;
	foo_conf conf;
	foo_conf conf_edit;

	bool ori_parsing_merge_titles = false;
	bool ori_parsing_merge_subtracks = false;
	bool ori_parsing_tracks_dotted_to_header = false;
	bool ori_parsing = false;
	bool ori_skip_video = false;

	bool setting_dlg = false;
	bool cancel_dlg = true;

	//todo:
	CHyperLink m_help_link_search;
	CHyperLink m_help_link_search_adv;
	CHyperLink m_help_link_matching;
	CHyperLink m_help_link_artwork;

	fb2k::CDarkModeHooks m_dark;

	CDialogResizeHelper m_resize_helper;

	HeaderStatic m_staticPrefHeader_Search1;          //IDC_GRP_STATIC_SEARCH_1
	HeaderStatic m_staticPrefHeader_Search2;
	HeaderStatic m_staticPrefHeader_Search3;

	HeaderStatic m_staticPrefHeader_SearchAdv1;       //IDC_GRP_STATIC_SEARCH_ADV_1
	HeaderStatic m_staticPrefHeader_SearchAdv2;
	HeaderStatic m_staticPrefHeader_SearchAdv3;

	HeaderStatic m_staticPrefHeader_History;          //IDC_GRP_STATIC_HISTORY
	HeaderStatic m_staticPrefHeader_Other;


	HeaderStatic m_staticPrefHeader_Art_Album;        //IDC_GRP_STATIC_CFG_ART_ALBUM
	HeaderStatic m_staticPrefHeader_Art_Artist;

	HeaderStatic m_staticPrefHeader_Oath_S1;          //IDC_GRP_STATIC_CFG_OATH_STEP_1
	HeaderStatic m_staticPrefHeader_Oath_S2;
	HeaderStatic m_staticPrefHeader_Oath_S3;

	HeaderStatic m_staticPrefHeader_Cache_Parse;      //IDC_GRP_STATIC_CFG_CACHE_PARSE
	HeaderStatic m_staticPrefHeader_Cache_Mem;        //IDC_GRP_STATIC_CFG_CACHE_MEM
	HeaderStatic m_staticPrefHeader_Cache_Disk;       //IDC_GRP_STATIC_CFG_CACHE_DISK

	HeaderStatic m_staticPrefHeader_Write_Tags;       //IDC_GRP_STATIC_CFG_WRITE_TAGS
	HeaderStatic m_staticPrefHeader_Artist_Names;     //IDC_GRP_STATIC_CFG_ARTIST_NAMES
	HeaderStatic m_staticPrefHeader_Multi_Values;     //IDC_GRP_STATIC_CFG_MULTIVALUES

	HeaderStatic m_staticPrefHeader_Trk_Matching;     //IDC_GRP_STATIC_TRK_MATCHING
	HeaderStatic m_staticPrefHeader_Trk_Auto_Matching;//IDC_GRP_STATIC_AUTO_TRK_MATCHING
	HeaderStatic m_staticPrefHeader_Trk_SB_Matching;  //IDC_GRP_STATIC_STATBAR_INFO_TRK_MATCHING
};

class preferences_page_myimpl : public preferences_page_impl<CConfigurationDialog> {

public:

	const char* get_name() { return COMPONENT_NAME_HC; }
	GUID get_guid() { return guid_pref_page; }
	GUID get_parent_guid() { return guid_tagging; }
};

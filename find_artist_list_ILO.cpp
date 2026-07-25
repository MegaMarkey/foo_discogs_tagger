#include "stdafx.h"

#include "find_artist_list.h"
#include "find_artist_list_ILO.h"
#include "find_release_dialog.h"

CArtistList* ILOD_artist_list::get_uilist() {

	return dynamic_cast<CArtistList*>(ilo_get_uilist());
}

// IListControlOwnerDataSource overrides

size_t ILOD_artist_list::listGetItemCount(ctx_t ctx) {

	return get_uilist()->Get_Artists().get_count();
}

pfc::string8 ILOD_artist_list::listGetSubItemText(ctx_t ctx, size_t item, size_t subItem) {

	auto artists = get_uilist()->Get_Artists();
	if (item < artists.get_count()) {
		return artists[item]->name;
	}
	else {
		return "";
	}
}

void ILOD_artist_list::listItemAction(ctx_t, size_t) {

	get_uilist()->Default_Action();
}

void ILOD_artist_list::listSelChanged(ctx_t) {

	if (!get_uilist()->GetSelectedCount()) {
		return;
	}

	cupdRelSrc in_cupdsrc(updRelSrc::ArtistProfile);

	in_cupdsrc.extended = CONF.auto_load_releases_on_select_ready();

	auto dlg = dynamic_cast<CFindReleaseDialog*>(this);

	if (CONF.auto_load_releases_on_select_ready()) {

		if (search_query::IsRunnable(dlg->m_search_expression)) {
			dlg->m_query_mode |= SearchMode::AT;
		}
	}
	dlg->convey_artist_list_selection(in_cupdsrc);
}

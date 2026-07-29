#pragma once

enum SearchMode {

	DEFAULT_SEARCH = 0,
	VA = (1 << 0),
	AT = (1 << 1),
	VA_AUTO_LOAD = (1 << 2),

};

static const std::vector<std::string> kv_query_fields_ordered{
		{"type="},
		{"q="},
		{"artist="},
		{"title="},
		{"track="},
		{"anv="},
		{"credit="},
		{"barcode="},
		{"catno="},
		{"label="},
		{"genre="},
		{"style="},
		{"format="},
		{"year="},
		{"country="},
		{"submitter="},
		{"contributor="},
		{"matrix="},
		{"needs_vote="},
		{"needs_changes="},
};

static const std::vector<std::string> kv_query_main_fields {
		{"artist="},
		{"title="},
		{"format="},
		{"year="},
		{"track="},
		{"credit="},
};

static const pfc::string8 DISCOGS_PUBLIC_SEARCH_URL = "https://www.discogs.com/search/";

//check various artists prefixes
extern pfc::string8 has_csv_prefix(const pfc::string& field, const pfc::string& csv, bool only_first_word, bool exact);
//check multiple artists joins
extern bool has_csv_links(pfc::string& field, const pfc::string& csv, bool replace);

using FieldValPair = std::pair<std::string, std::string>;
using QueryDefMap = std::map<std::string, FieldValPair>;

namespace search_query {

	inline const pfc::string8 k_ukm_artist_name = "Undefined masters";
	inline const pfc::string8 k_uk_artist_name = "Undefined releases";

	//max_24bits
	inline const size_t k_undefined_id_max = static_cast<size_t>(pow(2, 24));

	inline const pfc::string8 k_uk_id = PFC_string_formatter() << (k_undefined_id_max - 1);
	inline const pfc::string8 k_ukm_id = PFC_string_formatter() << (k_undefined_id_max - 2);

	//is this master/release undefined?
	inline bool is_undefined_artist(pfc::string8 id, bool bmasters) {
		return id.equals(bmasters? k_ukm_id : k_uk_id);
	}

	inline const size_t k_nested_suggestions = 5;

	bool MapToText(QueryDefMap qdm_search_query, std::pair<std::string, std::string>& out, bool alltypes = false, bool artist_or_q = true);
	bool TextToMap(const pfc::string8 expr, QueryDefMap& qdm_search_query);
	bool UrlToMap(const pfc::string8 expr, QueryDefMap& qdm_search_query);

	bool IsMinimal(const QueryDefMap qdm_search_query);
	pfc::string8 IsMinimalHint(const QueryDefMap qdm_search_query);
	bool isVAQueryMinimal(const QueryDefMap qdm_search_query);

	bool IsRunnable(const pfc::string8 search_expression);

	size_t CountAvailableFields(const QueryDefMap qdm_search_query);

	bool sanitize_various_artists_with_csv(const std::string& csv, pfc::string8& out);
	bool GetSuggestion(pfc::string8 frm_album_artist, pfc::string8 frm_artist, pfc::string8 frm_album, pfc::string8 frm_track_title, pfc::string8 frm_sani_artist, pfc::string8 frm_custom_tf, size_t ndx, pfc::string8& out);
	//todo: depricate
	bool TestArtistInTitle(pfc::string8& frm_album_artist, pfc::string8& frm_artist, pfc::string8& frm_album,
		pfc::string8& frm_track_title, const pfc::string& csv);

}
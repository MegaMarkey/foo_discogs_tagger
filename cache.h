#pragma once
#include <mutex>

template<typename key_t, typename value_t>

class lru_cache
{
public:
	typedef typename std::pair<key_t, value_t> key_value_pair_t;
	typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

	lru_cache(size_t max_size) :
		_max_size(max_size) {
	}

	std::mutex modify_mutex;

	bool remove(const key_t& key) {

		std::lock_guard<std::mutex> ul(modify_mutex);

		return remove_unlocked(key);
	}

	void put(const key_t& key, const value_t& value) {

		std::lock_guard<std::mutex> ul(modify_mutex);

		remove_unlocked(key);

		_cache_items_list.push_front(key_value_pair_t(key, value));
		_cache_items_map[key] = _cache_items_list.begin();

		if (_cache_items_map.size() > _max_size) {
			auto last = _cache_items_list.end();
			last--;
			_cache_items_map.erase(last->first);
			_cache_items_list.pop_back();
		}
	}

	const value_t& get(const key_t& key) {

		std::lock_guard<std::mutex> ul(modify_mutex);

		auto it = _cache_items_map.find(key);
		if (it == _cache_items_map.end()) {
			throw ("cache error");
		}
		else {
			_cache_items_list.splice(_cache_items_list.begin(), _cache_items_list, it->second);
			return it->second->second;
		}
	}

	// find and copy under lock (exists() + get() is not atomic)
	bool try_get(const key_t& key, value_t& out) {

		std::lock_guard<std::mutex> ul(modify_mutex);

		auto it = _cache_items_map.find(key);
		if (it == _cache_items_map.end()) {
			return false;
		}
		_cache_items_list.splice(_cache_items_list.begin(), _cache_items_list, it->second);
		out = it->second->second;
		return true;
	}

	bool exists(const key_t& key) /*const*/ {

		std::lock_guard<std::mutex> ul(modify_mutex);

		return _cache_items_map.find(key) != _cache_items_map.end();
	}

	std::pair<bool,bool> exists_comp(const key_t& key, key_t &alter_lkey) /*const*/ {

		std::lock_guard<std::mutex> ul(modify_mutex);

		std::pair<int, unsigned long> pdec = decode_mr(key);
		std::pair<bool, bool> pres(false, false);
		/*exist_simple*/ pres.first = _cache_items_map.find(pdec.second) != _cache_items_map.end();
		/*exist_comp*/ pres.second= _cache_items_map.find(key) != _cache_items_map.end();
		
		if (!pres.first && !pres.second) {

			size_t myres = 0;
			auto fi = std::find_if(_cache_items_map.begin(), _cache_items_map.end(), [&myres, &pdec](const auto /*std::pair<size_t, MasterRelease_ptr>*/ /*lru_cache<key_t, value_t>*/& e) {
				auto dec = decode_mr(e.first);
				if (dec.second == pdec.second) myres = e.first;
				return dec.second == pdec.second;
				});

			if (fi != _cache_items_map.end()) {
				alter_lkey = myres;
			}
		}

		return pres;

	}


	size_t size() /*const*/ {

		std::lock_guard<std::mutex> ul(modify_mutex);

		return _cache_items_map.size();
	}

	void clear() {

		std::lock_guard<std::mutex> ul(modify_mutex);

		_cache_items_list.clear();
		_cache_items_map.clear();
	}

	void set_max_size(size_t max_size) {

		std::lock_guard<std::mutex> ul(modify_mutex);

		_max_size = max_size;
		while (_cache_items_map.size() > _max_size) {
			auto last = _cache_items_list.end();
			last--;
			_cache_items_map.erase(last->first);
			_cache_items_list.pop_back();
		}
	}

private:

	// caller must hold modify_mutex
	bool remove_unlocked(const key_t& key) {

		//note c++ std::list & std::map: only those iterators, or refs pointing to
		//the element which will be erased, are affected

		auto it = _cache_items_map.find(key);
		if (it != _cache_items_map.end()) {
			_cache_items_list.erase(it->second);
			_cache_items_map.erase(it);
			return true;
		}
		return false;
	}

	std::list<key_value_pair_t> _cache_items_list;
	std::unordered_map<key_t, list_iterator_t> _cache_items_map;
	size_t _max_size;
};

namespace std
{
	template <>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	struct hash<pfc::string8> 
#else
	struct hash<pfc::string8> : public unary_function<pfc::string8, size_t>
#endif
	{
		size_t operator()(const pfc::string8& value) const {
			const char *str = value.get_ptr();
			unsigned long hash = 5381;
			int c;
			while (c = *str++) {
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
			}
			return hash;
		}
	};

	template <>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	struct equal_to<pfc::string8>
#else
	struct equal_to<pfc::string8> : public unary_function<pfc::string8, bool>
#endif
	{
		bool operator()(const pfc::string8& x, const pfc::string8& y) const {
			return STR_EQUAL(x, y);
		}
	};
}

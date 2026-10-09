#pragma once

// Reconstructed: tasks.cpp includes this header, but it was never committed
// upstream. Standard CRC-32 (ISO-HDLC, reflected polynomial 0xEDB88320, as used
// by zlib) over an iterator range of bytes; used to key search query history.

#include <array>
#include <cstdint>

namespace crc_detail {

	inline const std::array<std::uint32_t, 256>& crc32_table() {
		static const std::array<std::uint32_t, 256> table = [] {
			std::array<std::uint32_t, 256> t{};
			for (std::uint32_t i = 0; i < 256; ++i) {
				std::uint32_t c = i;
				for (int k = 0; k < 8; ++k) {
					c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
				}
				t[i] = c;
			}
			return t;
		}();
		return table;
	}
}

template <typename InputIterator>
std::uint32_t crc(InputIterator first, InputIterator last) {
	const auto& table = crc_detail::crc32_table();
	std::uint32_t c = 0xFFFFFFFFu;
	for (; first != last; ++first) {
		c = table[(c ^ static_cast<std::uint8_t>(*first)) & 0xFFu] ^ (c >> 8);
	}
	return c ^ 0xFFFFFFFFu;
}

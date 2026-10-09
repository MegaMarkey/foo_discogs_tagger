#pragma once

// Reconstructed: discogs.h and tasks.h include this header, but its current
// upstream version was never committed. db_fetcher is restored from the last
// committed version (upstream af5b91a^); the forward declaration is required
// by foo_discogs.h, which only sees this header through discogs.h.

class db_fetcher {
	//..
};

class foo_discogs_threaded_locked_process_callback;

#pragma once

#include "exception.h"
#include "utils.h"

class ErrorManager
{
public:
	virtual ~ErrorManager() {
	}

protected:
	bool fatal_error = false;
	pfc::array_t<pfc::string8> errors;

	void add_error(const char* msg, bool fatal = false) {
		fatal_error = fatal_error || fatal;
		pfc::string8 error;
		error << (fatal ? "(stopped) " : "(skipped) ") << msg;
		errors.append_single(error);
	}

	void add_error(foo_discogs_exception &e, bool fatal = false) {
		fatal_error = fatal_error || fatal;
		pfc::string8 error;
		error << (fatal ? "(stopped) " : "(skipped) ") << e.what();
		errors.append_single(error);
	}

	void add_error(const char* msg, foo_discogs_exception &e, bool fatal = false) {
		fatal_error = fatal_error || fatal;
		pfc::string8 error;
		error << (fatal ? "(stopped) " : "(skipped) ") << "[" << msg << "]: " << e.what();
		errors.append_single(error);
	}

	// displays errors in popup
	void display_errors() {
		if (errors.get_count()) {
			errors.append_single(pfc::string8("\n[ESCAPE to close]"));
			popup_message::g_show(join(errors, "\n").get_ptr(), "Exception", popup_message::icon_error);
		}
	}

	void clear_errors() {
		errors.force_reset();
	}
};

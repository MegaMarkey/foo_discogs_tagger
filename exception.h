#pragma once

#include <exception>
#include <sstream>
#include <string>


class foo_discogs_exception : public std::exception
{
protected:
	mutable std::string _mwhat;
	mutable std::ostringstream *_mstream;

public:
	foo_discogs_exception() : _mstream(nullptr) {}

	foo_discogs_exception(const char* msg) : _mwhat(msg), _mstream(nullptr) {}

	foo_discogs_exception(const foo_discogs_exception &that) {
		if (that._mstream != nullptr) {
			_mwhat = that._mstream->str();
		}
		else {
			_mwhat = that._mwhat;
		}
		_mstream = nullptr;
	}

	foo_discogs_exception& operator=(const foo_discogs_exception &that) {
		if (this != &that) {
			//copy the message, the stream is owned
			if (that._mstream != nullptr) {
				_mwhat = that._mstream->str();
			}
			else {
				_mwhat = that._mwhat;
			}
			delete _mstream;
			_mstream = nullptr;
		}
		return *this;
	}

	~foo_discogs_exception() {
		if (_mstream != nullptr) {
			delete _mstream;
		}
	}

	virtual const char *what() const override {
		if (_mstream != nullptr) {
			//keep the stream content, copies and later << read it
			//assign only on change, keeps pointers returned by earlier what() calls valid
			std::string s = _mstream->str();
			if (s != _mwhat) {
				_mwhat = s;
			}
		}
		return _mwhat.c_str();
	}

	template<typename T>
	foo_discogs_exception& operator<<(const T& t) {
		if (_mstream == nullptr) {
			_mstream = new std::ostringstream();
			(*_mstream) << _mwhat;
		}
		(*_mstream) << t;
		return (*this);
	}
};

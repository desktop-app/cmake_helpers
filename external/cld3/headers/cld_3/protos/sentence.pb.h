// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <string>

// Stand-in for code protoc generates from cld3/src/sentence.proto.
// CLD3 never serializes its messages, of the whole Sentence only text is read.

namespace chrome_lang_id {

class Sentence final {
public:
	[[nodiscard]] const std::string &text() const {
		return _text;
	}
	void set_text(const std::string &value) {
		_text = value;
	}

private:
	std::string _text;

};

} // namespace chrome_lang_id

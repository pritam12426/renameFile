#include "functions.hpp"

#include <regex>

// Escape characters that break std::regex
static inline std::string escapeRegex(char c) {
	static const std::string specials = ".^$|()[]{}*+?\\";
	return (specials.find(c) != std::string::npos) ? ("\\" + std::string(1, c))
	                                               : std::string(1, c);
}

bool formatPathName(fs::path &newString) {
	// Convert char8_t* → char safely (no UB)
	std::u8string u8s = newString.u8string();
	std::string s(reinterpret_cast<const char *>(u8s.c_str()));

	if (s.empty()) return false;

	// ---------------------- PRECOMPILED REGEX ----------------------
	static const std::regex nonAscii("[^\\x00-\\x7F]+");
	static const std::regex multiUnderscore("_+");
	static const std::regex multiSpace(" +");

	// Remove non-ASCII characters
	s = std::regex_replace(s, nonAscii, "");

	// Collapse multiple underscores -> single
	s = std::regex_replace(s, multiUnderscore, "_");

	// Replace multiple spaces -> underscore
	s = std::regex_replace(s, multiSpace, "_");

	// If nothing left, early exit
	if (s.empty()) {
		newString = fs::path("");
		return false;
	}

	// ---------------------- SPECIAL CHARACTER CLEANING ----------------------
	static const std::string special = "!#$%&'()*,-./:;<=>?@[]^_`{|}~";

	for (char c : special) {
		const std::string esc = escapeRegex(c);
		const std::string lit(1, c);

		// "_X" → "X"
		s = std::regex_replace(s, std::regex("_" + esc), lit);

		// "X_" → "X"
		s = std::regex_replace(s, std::regex(esc + "_"), lit);

		// "XX" → "X"
		s = std::regex_replace(s, std::regex(esc + esc), lit);
	}

	//  CLEAN BOUNDARIES & FINAL TOUCHES
	if (!s.empty() && s.front() == '_') s.erase(0, 1);
	if (!s.empty() && s.back() == '_') s.pop_back();

	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	std::replace(s.begin(), s.end(), '.', '_');

	// Update final path
	newString = fs::path(s);

	return !s.empty();
}

;

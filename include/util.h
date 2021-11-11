#ifndef LABCALC_UTIL_H
#define LABCALC_UTIL_H

#include <string>


/*
 * Return whether the string contains whitespace
 */

inline bool has_whitespace(const std::string& str) {

	for (int i = 0; i < str.size(); ++i) {
		if(str[i] == ' ' || str[i] == '\t')
			return true;
	}

	return false;
}


/*
 * Replace every occurrence of a string with another string
 */

inline void replace_string(
		std::string& str,
		const std::string& old_str,
		const std::string& new_str) {

	const size_t old_length = old_str.length();
	int pos = -1;

	while((pos = str.find(old_str, 0)) != std::string::npos) {
		str.replace(pos, old_length, new_str);
	}
}


/*
 * Convert upper case characters to lowercase
 */

inline std::string to_lower_case(std::string str) {

	std::string res;

	for (int i = 0; i < str.size(); ++i) {

		if(str[i] >= 'A' && str[i] <= 'Z')
			res += (str[i] - 'A' + 'a');
		else
			res += str[i];
	}

	return res;
}


#endif

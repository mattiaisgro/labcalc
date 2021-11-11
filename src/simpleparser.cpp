#include "simpleparser.h"
#include <sstream>
#include <algorithm>


void parse(LCSymList& symbols, std::string line, bool skip_spaces) {

	LCSymList tokens;

	// Replace tabs with spaces
	std::replace(line.begin(), line.end(), '\t', ' ');

	// Divide line by whitespaces
	std::stringstream stream(line);
	std::string token;

	while (getline(stream, token, ' '))
		tokens.emplace_back(std::move(token));

	// Recompose the tokens as symbols using quotes
	for (int i = 0; i < tokens.size(); ++i) {

		// Skip whitespace
		if(!tokens[i].size()) {
			if(skip_spaces)
				continue;
			else {
				symbols.emplace_back(" ");
				continue;
			}
		}

		if(tokens[i].at(0) == '\"') {

			if(tokens[i].at(tokens[i].size() - 1) == '\"') {
				tokens[i].erase(tokens[i].size() - 1, 1);
				tokens[i].erase(0, 1);
				symbols.emplace_back(tokens[i]);
				continue;
			}

			std::string curr = tokens[i];
			bool closed = false;

			for (int j = i + 1; j < tokens.size(); ++j) {

				curr += ' ';
				curr += tokens[j];

				if(tokens[j].at(tokens[j].size() - 1) == '\"') {

					curr.erase(curr.size() - 1, 1);
					curr.erase(0, 1);

					symbols.emplace_back(curr);
					closed = true;
					i = j;
					break;
				}
			}

			if(!closed) {
				// TO-DO throw...
				return;
			}

		} else {
			symbols.emplace_back(tokens[i]);
		}
	}
}

#include "commands.h"
#include "interpreter.h"
#include "simpleparser.h"

#include "common.h"
#include "vec_buff.h"
using uroboro::vec_buff;

#include "environment.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>


extern LCCommandTable commtable;


// Substitute script arguments in the form "%ARGx" with args[x - 1]
int sub_args(LCSymList& symbols, const LCSymList& args) {

	for (int i = 0; i < symbols.size(); ++i) {
		
		if(symbols[i].size() > 4) {

			for (int j = 0; j < symbols[i].size(); ++j) {

				if(j + 4 > symbols[i].size())
					break;
				
				if(symbols[i][j] == '$' &&
				symbols[i][j + 1] == 'A' &&
				symbols[i][j + 2] == 'R' &&
				symbols[i][j + 3] == 'G') {

					std::string index_str;

					for (j += 4; j < symbols[i].size(); ++j) {
						if(symbols[i][j] >= '0' && symbols[i][j] <= '9') {
							index_str += symbols[i][j];
						} else {
							break;
						}
					}

					unsigned int index = std::stoi(index_str);

					if(index > args.size() || index == 0) {
						std::cout << "Undefined argument $ARG" << index << std::endl;
						return -1;
					} else {
						replace_string(symbols[i], "$ARG" + index_str, args[index - 1]);

						// Start again from j = 0
						// TO-DO Improve algorithm
						j = -1;
					}
				}
			}
		}
	}
}


void lc_quit(const LCSymList& symbols, LCEnv& env) {

	// TO-DO
	// Ask whether to save datasets before exiting

	if(symbols.size() > 1) {
		exit(std::stoi(symbols[1]));
	} else {
		exit(0);
	}
}


void lc_insert(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'insert' needs at least one argument" << std::endl;
		return;
	}

	std::string curr;

	// Insert only one dataset
	if(symbols.size() == 2) {

		std::string name = symbols[1];
		bool overwrite = false;

		if(env_exists(env, name)) {
			if(env_get(env, "$OPTIONS")[OPT_ALWAYS_OVERWRITE] == 0) {
				std::cout << "The dataset " << name << " already exists" << std::endl;
				std::cout << "Do you want to overwrite it? (y/n)" << std::endl;

				getline(std::cin, curr);
				curr = to_lower_case(curr);

				if(curr == "y") {
					overwrite = true;
				} else if(curr == "n") {
					return;
				} else {
					std::cout << "Malformed answer, the buffer will not be overwritten" << std::endl;
					return;
				}
			} else {
				overwrite = true;
			}
		}

		std::cout << "Inserting " << name << std::endl;

		vec_buff buffer;

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			buffer.emplace_back(std::stod(curr));
		}

		// Dataset with <name> key already exists
		if(overwrite) {
			// Overwrite buffer
			env_get(env, name) = buffer;
		} else {
			// Dataset doesn't exist yet
			env_insert(env, name, buffer);
		}

	} else {

		// Insert a table

		size_t data_n = symbols.size() - 1;

		vec_buff buffers[symbols.size() - 1];
		bool overwrite[symbols.size() - 1] = { false };

		for (int i = 0; i < data_n; ++i) {
			if(env_exists(env, symbols[i + 1])) {
				if(env_get(env, "$OPTIONS")[OPT_ALWAYS_OVERWRITE] == 0) {
					std::cout << "The dataset " << symbols[i + 1] << " already exists" << std::endl;
					std::cout << "Do you want to overwrite it? (y/n)" << std::endl;

					getline(std::cin, curr);
					curr = to_lower_case(curr);

					if(curr == "y") {
						overwrite[i] = true;
					} else if(curr == "n") {
						std::cout << "The dataset will not be overwritten" << std::endl;
						std::cout << "A space in its corresponding column shall be written" << std::endl;
					} else {
						std::cout << "Malformed answer, the buffer will not be overwritten" << std::endl;
						return;
					}
				} else {
					overwrite[i] = true;
				}
			}
		}

		std::cout << "Inserting ";
		for (int i = 1; i < symbols.size(); ++i) {

			std::cout << symbols[i];

			if(i != symbols.size() - 1)
				std::cout << ", ";
		}
		std::cout << ":" << std::endl;

		LCSymList values;

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			values.clear();
			parse(values, curr, false);

			// Check for empty places
			for (int i = 0; i < values.size(); ++i) {
				if(values[i] == " " && i < values.size() - 1) {
					if(values[i + 1] == " ") {
						values.erase(values.begin() + i);
					}
				}
			}

			// Last column value may be omitted
			if(values.size() > data_n) {
				std::cout << "Malformed input, skipping current line..." << std::endl;
				continue;
			}

			for (int i = 0; i < values.size(); ++i) {

				// Replace ',' with '.' for compatibility
				// with European standard
				std::replace(values[i].begin(), values[i].end(), ',', '.');

				if(values[i] != " ")
					buffers[i].emplace_back(std::stod(values[i]));
			}
		}

		for (int i = 0; i < data_n; ++i) {
			if(overwrite[i]) {
				env_get(env, symbols[i + 1]) = buffers[i];
			} else if(!env_exists(env, symbols[i + 1])) {
				env_insert(env, symbols[i + 1], buffers[i]);
			}
		}
	}
}


void lc_append(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'append' needs at least one argument" << std::endl;
		return;
	}

	std::string curr;

	// Insert only one dataset
	if(symbols.size() == 2) {

		if(!env_exists(env, symbols[1])) {
			std::cout << "The dataset " << symbols[1] << " doesn't exist" << std::endl;
			return;
		} else {
			std::cout << "Appending values to " << symbols[1] << std::endl;
		}

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			env_get(env, symbols[1]).emplace_back(std::stod(curr));
		}

	} else {

		// Insert a table

		size_t data_n = symbols.size() - 1;

		for (int i = 1; i < data_n; ++i) {
			if(!env_exists(env, symbols[i])) {
				std::cout << "The dataset " << symbols[i] << " doesn't exist" << std::endl;
				return;
			}
		}


		std::cout << "Appending values to ";
		for (int i = 1; i < symbols.size(); ++i) {

			std::cout << symbols[i];

			if(i != symbols.size() - 1)
				std::cout << ", ";
		}
		std::cout << ":" << std::endl;

		LCSymList values;

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			values.clear();
			parse(values, curr, false);

			// Check for empty places
			for (int i = 0; i < values.size(); ++i) {
				if(values[i] == " " && i < values.size() - 1) {
					if(values[i + 1] == " ") {
						values.erase(values.begin() + i);
					}
				}
			}

			// Last column value may be omitted
			if(values.size() < (data_n - 1) || values.size() > data_n) {
				std::cout << "Malformed input, skipping current line..." << std::endl;
				continue;
			}

			for (int i = 0; i < values.size(); ++i) {

				// Replace ',' with '.' for compatibility
				// with European standard
				std::replace(values[i].begin(), values[i].end(), ',', '.');

				if(values[i] != " ")
					env_get(env, symbols[i + 1]).emplace_back(std::stod(values[i]));
			}
		}
	}
}


void lc_prepend(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'prepend' needs at least one argument" << std::endl;
		return;
	}

	std::string curr;
	unsigned long int position = 0;

	// Insert only one dataset
	if(symbols.size() == 2) {

		if(!env_exists(env, symbols[1])) {
			std::cout << "The dataset " << symbols[1] << " doesn't exist" << std::endl;
			return;
		} else {
			std::cout << "Prepending values to " << symbols[1] << std::endl;
		}

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			env_get(env, symbols[1]).insert(env_get(env, symbols[1]).begin() + position, std::stod(curr));
			position++;
		}

	} else {

		// Insert a table

		size_t data_n = symbols.size() - 1;

		for (int i = 1; i < data_n; ++i) {
			if(!env_exists(env, symbols[i])) {
				std::cout << "The dataset " << symbols[i] << " doesn't exist" << std::endl;
				return;
			}
		}


		std::cout << "Prepending values to ";
		for (int i = 1; i < symbols.size(); ++i) {

			std::cout << symbols[i];

			if(i != symbols.size() - 1)
				std::cout << ", ";
		}
		std::cout << ":" << std::endl;

		LCSymList values;

		while(getline(std::cin, curr)) {

			if(curr == "" || to_lower_case(curr) == "end")
				break;

			values.clear();
			parse(values, curr, false);

			// Check for empty places
			for (int i = 0; i < values.size(); ++i) {
				if(values[i] == " " && i < values.size() - 1) {
					if(values[i + 1] == " ") {
						values.erase(values.begin() + i);
					}
				}
			}

			// Last column value may be omitted
			if(values.size() < (data_n - 1) || values.size() > data_n) {
				std::cout << "Malformed input, skipping current line..." << std::endl;
				continue;
			}

			for (int i = 0; i < values.size(); ++i) {

				// Replace ',' with '.' for compatibility
				// with European standard
				std::replace(values[i].begin(), values[i].end(), ',', '.');

				if(values[i] != " ") {
					env_get(env, symbols[i + 1]).insert(
						env_get(env, symbols[i + 1]).begin() + position,
						std::stod(values[i]));
				}
			}

			position++;
		}
	}
}


void lc_delete(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'delete' needs at least one argument" << std::endl;
		return;
	}

	for (int i = 1; i < symbols.size(); ++i) {
		
		if(!env_exists(env, symbols[i])) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
		} else {
			env_erase(env, symbols[i]);
		}
	}
}


void lc_erase_pos(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'erase-pos' needs at least two arguments (dataset, position(s) starting at 1)" << std::endl;
		return;
	}

	if(!env_exists(env, symbols[1])) {
		std::cout << "Can't find dataset " << symbols[1] << std::endl;
		return;
	}

	vec_buff& v = env_get(env, symbols[1]);

	// TO-DO order positions in decreasing order
	// to make sure all values get erased correctly

	for (int i = 2; i < symbols.size(); ++i) {

		unsigned long int position = std::stoi(symbols[i]) - 1;

		if(position >= v.size()) {
			std::cout << "The position argument exceeds the dataset length" << std::endl;
			return;
		}
		
		v.erase(v.begin() + position);
		std::cout << "Erased position " << (position + 1) << " in " << symbols[1] << std::endl;
	}
}


void lc_erase_value(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'erase-value' needs two arguments (dataset, value to erase)" << std::endl;
		return;
	}

	if(!env_exists(env, symbols[1])) {
		std::cout << "Can't find dataset " << symbols[1] << std::endl;
		return;
	}

	vec_buff& v = env_get(env, symbols[1]);
	double value = std::stod(symbols[2]);
	auto position = std::find(v.begin(), v.end(), value);

	if(position == v.end()) {
		std::cout << "The value " << value << " can't be found inside the dataset " << symbols[1] << std::endl;
		return;
	}
	
	v.erase(position);
	std::cout << "Erased " << value << std::endl; // at position ...
}


void lc_erase_all_values(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'erase-all-values' needs at least two arguments (dataset, value(s) to erase)" << std::endl;
		return;
	}

	if(!env_exists(env, symbols[1])) {
		std::cout << "Can't find dataset " << symbols[1] << std::endl;
		return;
	}

	vec_buff& v = env_get(env, symbols[1]);
	vec_buff values;

	unsigned int occ = 0;

	for (int i = 2; i < symbols.size(); ++i)
		values.emplace_back(std::stoi(symbols[i]));
	

	for (int i = 0; i < v.size(); ++i) {

		for (int j = 0; j < values.size(); ++j) {
			if(v[i] == values[j]) {
				v.erase(v.begin() + i);
				i--;
				occ++;
				break;
			}
		}
	}

	std::cout << "Erased " << occ << " occurrences" << std::endl;
}


void lc_find(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'find' needs two arguments (dataset, value to find)" << std::endl;
		return;
	}

	if(!env_exists(env, symbols[1])) {
		std::cout << "Can't find dataset " << symbols[1] << std::endl;
		return;
	}

	vec_buff& v = env_get(env, symbols[1]);
	double value = std::stod(symbols[2]);
	std::vector<unsigned long int> positions;

	// Find occurrences of <value>
	for (int i = 0; i < v.size(); ++i) {
		if(v[i] == value) {
			positions.emplace_back(i + 1);
		}
	}

	// No occurrences found
	if(!positions.size()) {
		std::cout << "The value " << value << " can't be found inside the dataset " << symbols[1] << std::endl;
		return;
	}

	// Print found positions
	for (int i = 0; i < positions.size(); ++i) {
		std::cout << positions[i] << " ";
	}
	std::cout << std::endl;
}


void lc_load(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'load' needs at least one argument" << std::endl;
		return;
	}

	for (int i = 1; i < symbols.size(); ++i) {

		std::ifstream file(symbols[i]);
		unsigned long int file_line = 0;

		if(!file.is_open()) {
			std::cout << "Unable to open data file: " << symbols[i] << std::endl;
			return;
		}

		bool comment = true;
		std::string curr;

		// Read lines until the header is found
		do {

			if(!file || file.eof()) {
				comment = false;
				curr = "";
				break;
			}

			getline(file, curr);
			file_line++;

			std::replace(curr.begin(), curr.end(), '\t', ' ');

			if(!curr.size())
				continue;

			// Check if the line is commented
			for (int j = 0; j < curr.size(); ++j) {

				if(curr[j] == ' ') {
					continue;
				} else if(curr[j] == '#') {
					comment = true;
					break;
				} else {
					comment = false;
					break;
				}
			}

		} while(comment);

		// Parse header
		LCSymList header;
		parse(header, curr);

		if(header.size() == 0) {
			std::cout << "The file " << symbols[i] << " is empty" << std::endl;
			continue;
		}

		vec_buff buffers[header.size()];
		LCSymList values;
		int overwrite[header.size()];

		// Initialize with 2
		for (int& i : overwrite)
			i = 2;

		// 0 = exists, do not overwrite
		// 1 = exists, overwrite
		// 2 = does not exist, create

		// Check for already existing datasets with same name
		
		for (int i = 0; i < header.size(); ++i) {
			if(env_exists(env, header[i])) {
				if(env_get(env, "$OPTIONS")[OPT_ALWAYS_OVERWRITE] == 0) {
					std::cout << "The dataset " << header[i] << " already exists" << std::endl;
					std::cout << "Do you want to overwrite it? (y/n)" << std::endl;

					getline(std::cin, curr);
					curr = to_lower_case(curr);

					if(curr == "y") {
						overwrite[i] = 1;
					} else if(curr == "n") {
						overwrite[i] = 0;
						std::cout << "The dataset will not be overwritten" << std::endl;
					} else {
						std::cout << "Malformed answer, the buffer will not be overwritten" << std::endl;
						return;
					}
				} else {
					overwrite[i] = 1;
				}
			}
		}

		// Read data from the file
		while (getline(file, curr)) {

			file_line++;

			std::replace(curr.begin(), curr.end(), '\t', ' ');

			// Replace ',' with '.' for compatibility
			// with European standard
			std::replace(curr.begin(), curr.end(), ',', '.');

			// Check if the line is commented
			comment = false;
			for (int i = 0; i < curr.size(); ++i) {

				if(curr[i] == ' ') {
					continue;
				} else if(curr[i] == '#') {
					comment = true;
					break;
				} else {
					break;
				}
			}

			// Skip line if commented
			if(comment)
				continue;

			values.clear();
			parse(values, curr, false);

			if(!values.size())
				continue;

			// Check for empty places
			for (int i = 0; i < values.size(); ++i) {
				if(values[i] == " " && i < values.size() - 1) {
					if(values[i + 1] == " ") {
						values.erase(values.begin() + i);
					}
				}
			}

			if(values.size() > header.size()) {
				std::cout << "Malformed input (line " << file_line <<"), closing file..." << std::endl;
				file.close();
				return;
			}

			for (int i = 0; i < uroboro::min(values.size(), header.size()); ++i) {

				if(values[i] != " ")
					buffers[i].emplace_back(std::stod(values[i]));
			}
		}


		// Insert datasets into hash map
		for (int i = 0; i < header.size(); ++i) {
			if(overwrite[i] == 1) {
				env_get(env, header[i]) = buffers[i];
			} else if(overwrite[i] == 2) {
				env_insert(env, header[i], buffers[i]);
			}
		}


		// Print loaded dataset keys
		std::cout << "Loaded: ";
		for (int j = 0; j < header.size(); ++j) {

			if(overwrite[j] && env_exists(env, header[j])) {
				std::cout << header[j];

				if(j != header.size() - 1)
					std::cout << ", ";
			}
		}
		std::cout << " from '" << symbols[i] << "'" << std::endl;
	}
}


void lc_load_as(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'load-as' needs at least one argument" << std::endl;
		return;
	}

	std::ifstream file(symbols[1]);
	unsigned long int file_line = 0;
	vec_buff buffers[symbols.size() - 2];

	if(!file.is_open()) {
		std::cout << "Unable to open data file: " << symbols[1] << std::endl;
		return;
	}

	bool comment = true;
	std::string curr;

	// Read lines until the header is found
	do {

		if(!file || file.eof()) {
			comment = false;
			curr = "";
			break;
		}

		getline(file, curr);
		file_line++;

		std::replace(curr.begin(), curr.end(), '\t', ' ');

		if(!curr.size())
			continue;

		// Check if the line is commented
		for (int i = 0; i < curr.size(); ++i) {

			if(curr[i] == ' ') {
				continue;
			} else if(curr[i] == '#') {
				comment = true;
				break;
			} else {
				comment = false;
				break;
			}
		}

	} while(comment);

	LCSymList values;
	int overwrite[symbols.size() - 2];

	// Initialize with 2
	for (int& i : overwrite)
		i = 2;

	// 0 = exists, do not overwrite
	// 1 = exists, overwrite
	// 2 = does not exist, create

	// Check for already existing datasets with same name
	for (int i = 2; i < symbols.size(); ++i) {
		if(env_exists(env, symbols[i])) {
			if(env_get(env, "$OPTIONS")[OPT_ALWAYS_OVERWRITE] == 0) {
				std::cout << "The dataset " << symbols[i] << " already exists" << std::endl;
				std::cout << "Do you want to overwrite it? (y/n)" << std::endl;

				getline(std::cin, curr);
				curr = to_lower_case(curr);

				if(curr == "y") {
					overwrite[i - 2] = 1;
				} else if(curr == "n") {
					overwrite[i - 2] = 0;
					std::cout << "The dataset will not be overwritten" << std::endl;
				} else {
					std::cout << "Malformed answer, the buffer will not be overwritten" << std::endl;
					return;
				}
			} else {
				overwrite[i - 2] = 1;
			}
		}
	}

	// Read data from the file
	while (getline(file, curr)) {

		file_line++;

		std::replace(curr.begin(), curr.end(), '\t', ' ');

		// Replace ',' with '.' for compatibility
		// with European standard
		std::replace(curr.begin(), curr.end(), ',', '.');

		// Check if the line is commented
		comment = false;
		for (int i = 0; i < curr.size(); ++i) {

			if(curr[i] == ' ') {
				continue;
			} else if(curr[i] == '#') {
				comment = true;
				break;
			} else {
				break;
			}
		}

		// Skip line if commented
		if(comment)
			continue;

		values.clear();
		parse(values, curr, false);

		if(!values.size())
			continue;

		// Check for empty places
		for (int i = 0; i < values.size(); ++i) {
			if(values[i] == " " && i < values.size() - 1) {
				if(values[i + 1] == " ") {
					values.erase(values.begin() + i);
				}
			}
		}

		for (int i = 0; i < uroboro::min(values.size(), symbols.size() - 2); ++i) {

			if(values[i] != " ")
				buffers[i].emplace_back(std::stod(values[i]));
		}
	}


	// Insert datasets into hash map
	for (int i = 2; i < symbols.size(); ++i) {
		if(overwrite[i - 2] == 1) {
			env_get(env, symbols[i]) = buffers[i - 2];
		} else if(overwrite[i - 2] == 2) {
			env_insert(env, symbols[i], buffers[i - 2]);
		}
	}


	// Print loaded dataset keys
	std::cout << "Loaded: ";
	for (int j = 2; j < symbols.size(); ++j) {

		if(overwrite[j] && env_exists(env, symbols[j])) {
			std::cout << symbols[j];

			if(j != symbols.size() - 1)
				std::cout << ", ";
		}
	}
	std::cout << " from '" << symbols[1] << "'" << std::endl;
}


void lc_save(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'save' needs at least two arguments (filename and dataset name)" << std::endl;
		return;
	}

	// Check for existence before creating file
	for (int i = 2; i < symbols.size(); ++i) {
		if(!env_exists(env, symbols[i])) {
			std::cout << "Unable to find dataset: " << symbols[i] << std::endl;
			return;
		}
	}

	std::ofstream out(symbols[1]);

	if(!out.is_open()) {
		std::cout << "Unable to open output file: " << symbols[1] << std::endl;
		return;
	}

	// Determine the maximum size of the datasets
	size_t max_size = 0;

	for (int i = 2; i < symbols.size(); ++i) {

		if(env_get(env, symbols[i]).size() > max_size)
			max_size = env_get(env, symbols[i]).size();
	}


	// Print table header
	for (int i = 2; i < symbols.size(); ++i) {

		// Check if quotes are needed
		if(has_whitespace(symbols[i])) {
			out << "\"" << symbols[i] << "\"";
		} else {
			out << symbols[i];
		}

		if(i != (symbols.size() - 1))
			out << " ";
	}

	out << "\n";


	// Print table by row
	for (int i = 0; i < max_size; ++i) {

		for (int j = 2; j < symbols.size(); ++j) {

			if(env_get(env, symbols[j]).size() > i) {
				out << env_get(env, symbols[j]).at(i);
			} else {
				out << " ";
			}

			if(j != (symbols.size() - 1))
				out << " ";
		}

		out << "\n";
	}

	out.close();
}


void lc_autosave(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'autosave' needs at least one argument (filename)" << std::endl;
		return;
	}

	std::fstream file(symbols[1]);

	if(!file.is_open()) {
		std::cout << "Unable to open file: " << symbols[1] << std::endl;
		return;
	}

	// !!!
	// TO-DO Skip comments and whitespace
	// !!!

	// Read and parse first line (header)
	std::string curr;
	getline(file, curr);
	file.close();

	LCSymList header;
	parse(header, curr);

	// Check for existence before clearing file
	for (int i = 0; i < header.size(); ++i) {
		if(!env_exists(env, header[i])) {
			std::cout << "Unable to find dataset: " << header[i] << std::endl;
			return;
		}
	}

	// Open output file
	std::ofstream out(symbols[1]);

	if(!out.is_open()) {
		std::cout << "Unable to open output file: " << symbols[1] << std::endl;
		return;
	}

	// Determine the maximum size of the datasets
	size_t max_size = 0;

	for (int i = 0; i < header.size(); ++i) {

		if(env_get(env, header[i]).size() > max_size)
			max_size = env_get(env, header[i]).size();
	}


	// Print table header
	for (int i = 0; i < header.size(); ++i) {

		// Check if quotes are needed
		if(has_whitespace(header[i])) {
			out << "\"" << header[i] << "\"";
		} else {
			out << header[i];
		}

		if(i != (header.size() - 1))
			out << " ";
	}

	out << "\n";


	// Print table by row
	for (int i = 0; i < max_size; ++i) {

		for (int j = 0; j < header.size(); ++j) {

			if(env_get(env, header[j]).size() > i) {
				out << env_get(env, header[j]).at(i);
			} else {
				out << " ";
			}

			if(j != (header.size() - 1))
				out << " ";
		}

		out << "\n";
	}

	out.close();
}


void lc_print(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'print' needs at least one argument" << std::endl;
		return;
	}

	// Determine the maximum size of the datasets
	size_t max_size = 0;

	for (int i = 1; i < symbols.size(); ++i) {

		// Check for existence
		if(!env_exists(env, symbols[i])) {
			std::cout << "Unable to find dataset: " << symbols[i] << std::endl;
			return;
		}

		if(env_get(env, symbols[i]).size() > max_size)
			max_size = env_get(env, symbols[i]).size();
	}


	// Print table header
	for (int i = 1; i < symbols.size(); ++i)
		std::cout << symbols[i] << "\t";

	std::cout << std::endl;


	// Print table by row
	for (int i = 0; i < max_size; ++i) {

		for (int j = 1; j < symbols.size(); ++j) {

			if(env_get(env, symbols[j]).size() > i) {
				std::cout << env_get(env, symbols[j]).at(i);
			}

			std::cout << "\t";
		}

		std::cout << std::endl;
	}
}


void lc_print_lines(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'print' needs at least one argument" << std::endl;
		return;
	}

	// Determine the maximum size of the datasets
	size_t max_size = 0;

	for (int i = 1; i < symbols.size(); ++i) {

		// Check for existence
		if(!env_exists(env, symbols[i])) {
			std::cout << "Unable to find dataset: " << symbols[i] << std::endl;
			return;
		}

		if(env_get(env, symbols[i]).size() > max_size)
			max_size = env_get(env, symbols[i]).size();
	}


	// Print table header
	std::cout << "\t";
	for (int i = 1; i < symbols.size(); ++i)
		std::cout << symbols[i] << "\t";

	std::cout << std::endl;


	// Print table by row
	for (int i = 0; i < max_size; ++i) {

		std::cout << (i + 1) << ":\t";

		for (int j = 1; j < symbols.size(); ++j) {

			if(env_get(env, symbols[j]).size() > i) {
				std::cout << env_get(env, symbols[j]).at(i);
			}

			std::cout << "\t";
		}

		std::cout << std::endl;
	}
}


void lc_exec(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << "'exec' needs at least one argument" << std::endl;
		return;
	}

	// Prepare argument list from symbols[i > 1]
	LCSymList args;
	for (int i = 2; i < symbols.size(); ++i) {
		args.push_back(symbols[i]);
	}

	// Read and execute file
	std::ifstream script(symbols[1]);

	if(!script.is_open()) {
		std::cout << "Unable to open script: " << symbols[1] << std::endl;
		return;
	}

	std::string line;
	while(getline(script, line)) {

		// Parse current line
		LCSymList script_symbols;
		parse(script_symbols, line);

		if(!script_symbols.size())
			continue;

		if(script_symbols[0][0] == '#')
			continue;

		// Substitute args
		sub_args(script_symbols, args);

		// Execute current line
		interpret(script_symbols, env);
	}

	script.close();
}


void lc_echo(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << std::endl;
		return;
	}

	for (int i = 1; i < symbols.size(); ++i) {
		std::cout << symbols[i];
		std::cout << " ";
	}

	std::cout << std::endl;
}


void lc_echo_noline(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() == 1) {
		std::cout << std::endl;
		return;
	}

	for (int i = 1; i < symbols.size(); ++i) {
		std::cout << symbols[i];

		if(i != symbols.size() - 1)
			std::cout << " ";
	}
}


void lc_rename(const LCSymList& symbols, LCEnv& env) {

	if(symbols.size() < 3) {
		std::cout << "'rename' needs at least two arguments (dataset name, new name)" << std::endl;
		return;
	}

	if(!env_exists(env, symbols[1])) {
		std::cout << "Can't find dataset " << symbols[1] << std::endl;
		return;
	}


	env_insert(env, symbols[2], env_get(env, symbols[1]));
	env_erase(env, symbols[1]);

	std::cout << "Renamed '" << symbols[1] << "' to '" << symbols[2] << "'" << std::endl;
}


void lc_options(const LCSymList& symbols, LCEnv& env) {

	vec_buff& options = env_get(env, "$OPTIONS");

	if(symbols.size() == 1) {

		// Print options

		std::cout << "output-precision = " << options[OPT_OUTPUT_PRECISION] << std::endl;
		std::cout << "silent-scripts = " << options[OPT_SILENT_SCRIPTS] << std::endl;
		std::cout << "always-overwrite = " << options[OPT_ALWAYS_OVERWRITE] << std::endl;

	} else if(symbols.size() == 2) {

		// Print specific option

		if(symbols[1] == "output-precision") {
			std::cout << options[OPT_OUTPUT_PRECISION] << std::endl;
		} else if(symbols[1] == "silent-scripts") {
			std::cout << options[OPT_SILENT_SCRIPTS] << std::endl;
		} else if(symbols[1] == "always-overwrite") {
			std::cout << options[OPT_ALWAYS_OVERWRITE] << std::endl;
		} else {
			std::cout << "Unknown option" << std::endl;
		}

	} else if(symbols.size() == 3) {

		// Set specific option

		double value = std::stod(symbols[2]);

		if(symbols[1] == "output-precision") {
			options[OPT_OUTPUT_PRECISION] = value;
			std::cout.precision(value);
			std::cout << "output-precision set to " << value << std::endl;
		} else if(symbols[1] == "silent-scripts") {
			options[OPT_SILENT_SCRIPTS] = value;
			std::cout << "silent-scripts set to " << value << std::endl;
		} else if(symbols[1] == "always-overwrite") {
			options[OPT_ALWAYS_OVERWRITE] = value;
			std::cout << "always-overwrite set to " << value << std::endl;
		} else {
			std::cout << "Unknown option" << std::endl;
		}
	}
}


void lc_random(const LCSymList& symbols, LCEnv& env) {

	// random a b

	// Random in interval [a, b]

	// return (rand() % (b - a)) + a;
}


void lc_random_set(const LCSymList& symbols, LCEnv& env) {

	// random setname size a b

	// Random set of size <size> in interval [a, b]

	// for i < size
	//	data.push_back((rand() % (b - a)) + a);

	// env_insert(env, setname, data);
}


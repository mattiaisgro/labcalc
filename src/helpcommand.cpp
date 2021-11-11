#include "helpcommand.h"
#include <iostream>


void lc_help(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'help' - Get information about labcalc" << std::endl;
		return;
	}

	// TO-DO
	if(symbols[1] == "insert") {

	} else if(symbols[1] == "load") {

	}

}

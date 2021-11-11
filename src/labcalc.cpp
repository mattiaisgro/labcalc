#define UROBORO_INCLUDE_ALL
#include "./statistics.h"
#include "./utility.h"

#include "simpleparser.h"
#include "interpreter.h"

#include <iostream>
#include <string>
#include <map>

using namespace uroboro;

constexpr int LABCALC_MAJOR = 0;
constexpr int LABCALC_MINOR = 1;
constexpr int LABCALC_PATCH = 0;


int main(int argc, char const *argv[]) {

	std::cout << "\n" << "  LabCalc Version " <<
		LABCALC_MAJOR << "." <<
		LABCALC_MINOR << "." <<
		LABCALC_PATCH << "\n" <<
		"  A free statistical analysis tool\n" << std::endl;


	std::string line;
	LCEnv env;

	// Initialize command table and options
	init_commandtable();
	init_options(env);

	// Initialize result vector
	env_set_res(env, {0.0});

	// Execute autoexec.lab
	lc_exec({"exec", "autoexec.lab"}, env);


	// Prompt
	while (true) {

		std::cout << "labcalc > ";

		// Get the next line from the prompt
		getline(std::cin, line);

		// Parse the current line into symbols
		LCSymList symbols;
		parse(symbols, line);

		// Newline or whitespace only
		if(!symbols.size())
			continue;

		// Interpret and execute line
		interpret(symbols, env);
	}

	return 0;
}

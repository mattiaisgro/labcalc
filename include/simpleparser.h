#ifndef LABCALC_SIMPLEPARSER
#define LABCALC_SIMPLEPARSER

#include "environment.h"


/*
 * Parse a line into a symbol list
 *
 * e.g. the string "exec "my script.lab" 1 2 3"
 * gets translated into
 * {"exec", "my script.lab", "1", "2", "3"}
 *
 * <skip_spaces> specifies whether to include multiple whitespace
 * as a " " symbol
 */

void parse(LCSymList& symbols, std::string line, bool skip_spaces = true);


#endif

#ifndef LABCALC_INTERPRETER
#define LABCALC_INTERPRETER

#include "commands.h"
#include "mathcommands.h"
#include "helpcommand.h"
#include "environment.h"


using LCCommand = void(*)(const LCSymList& symbols, LCEnv& env);
using LCCommandTable = std::map<std::string, LCCommand>;


void insert_command(const std::string& symbol, LCCommand command);


void init_commandtable();


void init_options(LCEnv& env);


void interpret(const LCSymList& symbols, LCEnv& env);


#endif

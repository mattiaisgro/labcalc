#ifndef LABCALC_COMMANDS_H
#define LABCALC_COMMANDS_H

#include "environment.h"

constexpr unsigned int OPT_OUTPUT_PRECISION = 0;
constexpr unsigned int OPT_SILENT_SCRIPTS = 1;
constexpr unsigned int OPT_ALWAYS_OVERWRITE = 2;


/*
 * Quit labcalc, returning an exit number if provided
 */

void lc_quit(const LCSymList& symbols, LCEnv& env);


/*
 * Insert (a) new dataset(s) with the given name(s)
 * If multiple datasets are entered, input is by row
 */

void lc_insert(const LCSymList& symbols, LCEnv& env);


/*
 * Append data to already existing datasets 
 */

void lc_append(const LCSymList& symbols, LCEnv& env);


/*
 * Prepend data to already existing datasets
 */

void lc_prepend(const LCSymList& symbols, LCEnv& env);


/*
 * Delete datasets
 */

void lc_delete(const LCSymList& symbols, LCEnv& env);


/*
 * Erase the n-th value of a dataset (starting at 1) 
 */

void lc_erase_pos(const LCSymList& symbols, LCEnv& env);


/*
 * Erase the first occurrence of a value inside a dataset
 */

void lc_erase_value(const LCSymList& symbols, LCEnv& env);


/*
 * Erase all occurrences of a value inside a dataset
 */

void lc_erase_all_values(const LCSymList& symbols, LCEnv& env);


/*
 * Find and return the index (starting at 1) of a value
 * inside a dataset
 */

void lc_find(const LCSymList& symbols, LCEnv& env);


/*
 * Load all datasets from the specified data files
 */

void lc_load(const LCSymList& symbols, LCEnv& env);


/*
 * Load datasets from a file using the specified names 
 */

void lc_load_as(const LCSymList& symbols, LCEnv& env);


/*
 * Save datasets to a file
 */

void lc_save(const LCSymList& symbols, LCEnv& env);


/*
 * Automatically update a file's datasets 
 */

void lc_autosave(const LCSymList& symbols, LCEnv& env);


/*
 * Print the specified datasets 
 */

void lc_print(const LCSymList& symbols, LCEnv& env);


/*
 * Print the specified datasets with line numbers 
 */

void lc_print_lines(const LCSymList& symbols, LCEnv& env);


/*
 * Execute a script with the specified arguments
 */

void lc_exec(const LCSymList& symbols, LCEnv& env);


/*
 * Print a string and a newline
 */

void lc_echo(const LCSymList& symbols, LCEnv& env);


/*
 * Print a string
 */

void lc_echo_noline(const LCSymList& symbols, LCEnv& env);


/*
 * Rename a dataset
 */

void lc_rename(const LCSymList& symbols, LCEnv& env);


/*
 * Show, set and get option values
 */

void lc_options(const LCSymList& symbols, LCEnv& env);


/*
 * Generate a random number in a given range
 */

void lc_random(const LCSymList& symbols, LCEnv& env);


/*
 * Generate a set of random values in a given range
 */

void lc_random_set(const LCSymList& symbols, LCEnv& env);


#endif

#ifndef LABCALC_ENVIRONMENT_H
#define LABCALC_ENVIRONMENT_H

#include <map>
#include <string>
#include "vec_buff.h"


using LCEnv = std::map<std::string, uroboro::vec_buff>;
using LCSymList = std::vector<std::string>;
using uroboro::vec_buff;


/*
 * Check if a symbol exists inside the environment
 */

inline bool env_exists(LCEnv& env, const std::string& symbol) {
	return env.find(symbol) != env.end();
}


/*
 * Get the specified dataset from the environment
 */

inline vec_buff& env_get(LCEnv& env, const std::string& symbol) {
	return env[symbol];
}


/*
 * Set a dataset with the given key
 */

inline void env_set(LCEnv& env, const std::string& symbol, const vec_buff& new_vec) {
	env[symbol] = new_vec;
}


/*
 * Set the result vector
 */

inline void env_set_res(LCEnv& env, const vec_buff& new_res) {
	env["%RES"] = new_res;
}


/*
 * Erase a symbol from the environment
 */

inline void env_erase(LCEnv& env, const std::string& symbol) {
	env.erase(symbol);
}


/*
 * Insert a symbol inside the environment
 */

inline void env_insert(LCEnv& env, const std::string& symbol, vec_buff& data) {
	env.insert(std::pair<std::string, vec_buff>(symbol, std::move(data)));
}


#endif

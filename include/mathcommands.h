#ifndef LABCALC_MATHCOMMANDS_H
#define LABCALC_MATHCOMMANDS_H

#define UROBORO_INCLUDE_ALL
#include "uroboro.h"

#include "environment.h"


/*
 * Return the size of a dataset
 */

void lcm_size(const LCSymList& symbols, LCEnv& env);


/*
 * Return the mean value of a dataset
 * Calculated as sum(x_i) / N
 */

void lcm_mean(const LCSymList& symbols, LCEnv& env);


/*
 * Return the variance of a dataset
 */

void lcm_variance(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_variance_sample(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_stdev(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_stdev_sample(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_stdom(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_stdom_sample(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_relative_error(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_relative_error_sample(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_linearize(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_correlation_coeff(const LCSymList& symbols, LCEnv& env);


/*
 * 
 */

void lcm_r_squared(const LCSymList& symbols, LCEnv& env);


#endif

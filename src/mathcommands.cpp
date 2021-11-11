#include "mathcommands.h"
#include <iostream>


void lcm_size(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'size' needs at least one argument" << std::endl;
		return;
	}

	size_t size = 0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		size = env[symbols[i]].size();
		std::cout << size << std::endl;
	}

	// Set the last size as result
	env_set_res(env, {(real) size});
}


void lcm_mean(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'mean' needs at least one argument" << std::endl;
		return;
	}

	real mean = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		mean = uroboro::mean(env[symbols[i]]);
		std::cout << mean << std::endl;
	}

	env_set_res(env, {mean});
}


void lcm_variance(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'variance' needs at least one argument" << std::endl;
		return;
	}

	real variance = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		variance = uroboro::variance(env[symbols[i]]);
		std::cout << variance << std::endl;
	}

	env_set_res(env, {variance});
}


void lcm_variance_sample(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'variance-sample' needs at least one argument" << std::endl;
		return;
	}

	real variance = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		variance = uroboro::sample_variance(env[symbols[i]]);
		std::cout << variance << std::endl;
	}

	env_set_res(env, {variance});
}


void lcm_stdev(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'stdev' needs at least one argument" << std::endl;
		return;
	}

	real stdev = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		stdev = uroboro::stdev(env[symbols[i]]);
		std::cout << stdev << std::endl;
	}

	env_set_res(env, {stdev});
}


void lcm_stdev_sample(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'stdev-sample' needs at least one argument" << std::endl;
		return;
	}

	real stdev = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		stdev = uroboro::smpl_stdev(env[symbols[i]]);
		std::cout << stdev << std::endl;
	}

	env_set_res(env, {stdev});
}


void lcm_stdom(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'stdom' needs at least one argument" << std::endl;
		return;
	}

	real stdom = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		stdom = uroboro::stdom(env[symbols[i]]);
		std::cout << stdom << std::endl;
	}

	env_set_res(env, {stdom});
}


void lcm_stdom_sample(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'stdom-sample' needs at least one argument" << std::endl;
		return;
	}

	real stdom = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		stdom = uroboro::smpl_stdom(env[symbols[i]]);
		std::cout << stdom << std::endl;
	}

	env_set_res(env, {stdom});
}


void lcm_relative_error(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'relative-error' needs at least one argument" << std::endl;
		return;
	}

	real rel_err = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		rel_err = uroboro::standard_relative_error(env[symbols[i]]);
		std::cout << rel_err * 100 << "%" << std::endl;
	}

	env_set_res(env, {rel_err});
}


void lcm_relative_error_sample(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 2) {
		std::cout << "'relative-error' needs at least one argument" << std::endl;
		return;
	}

	real rel_err = 0.0;

	for (int i = 1; i < symbols.size(); ++i) {

		if(env.find(symbols[i]) == env.end()) {
			std::cout << "Can't find dataset " << symbols[i] << std::endl;
			return;
		}

		rel_err = uroboro::sample_standard_relative_error(env[symbols[i]]);
		std::cout << rel_err * 100 << "%" << std::endl;
	}

	env_set_res(env, {rel_err});
}


void lcm_linearize(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 3) {
		std::cout << "'linearize' needs at least two arguments (X values, Y values, [sigmaY])" << std::endl;
		return;
	}

	if(env.find(symbols[1]) == env.end()) {
		std::cout << "The dataset " << symbols[1] << " couldn't be found" << std::endl;
		return;
	}

	if(env.find(symbols[2]) == env.end()) {
		std::cout << "The dataset " << symbols[2] << " couldn't be found" << std::endl;
		return;
	}

	vec_buff& X = env[symbols[1]];
	vec_buff& Y = env[symbols[2]];

	if(X.size() != Y.size() || X.size() == 0) {
		std::cout << "The X and Y datasets must have the same number of elements and must not be empty" << std::endl;
		return;
	}
 
	double intercept = uroboro::lst_sqrs_lin_intercept(X, Y);
	double slope = uroboro::lst_sqrs_lin_slope(X, Y);

	std::cout << "Linearization between '" << symbols[1] << "' and '" << symbols[2] << "':" << std::endl;

	if(symbols.size() == 3) {

		double sigmaY = uroboro::lst_sqrs_lin_error(X, Y, intercept, slope);
		double sigmaA = uroboro::least_squares_linear_sigma_A(X, Y, sigmaY);
		double sigmaB = uroboro::least_squares_linear_sigma_B(X, Y, sigmaY);

		std::cout << "Y = (" << intercept << " +- " << sigmaA << ") + (" <<
			slope << " +- " << sigmaB << ") * X" << std::endl;

		std::cout << "Intercept = " << intercept << " +- " << sigmaA << std::endl;
		std::cout << "Slope = " << slope << " +- " << sigmaB << std::endl;

		std::cout << "Lin. Error: " << sigmaY << std::endl;
		std::cout << "The linearization error was used to estimate SigmaA and SigmaB" << std::endl;

	} else {

		// Replace ',' with '.' for compatibility
		// with European standard
		std::string sigmaY_str = symbols[3];
		std::replace(sigmaY_str.begin(), sigmaY_str.end(), ',', '.');

		double sigmaY = std::stod(sigmaY_str);
		double sigmaA = uroboro::least_squares_linear_sigma_A(X, Y, sigmaY);
		double sigmaB = uroboro::least_squares_linear_sigma_B(X, Y, sigmaY);

		std::cout << "Y = (" << intercept << " +- " << sigmaA << ") + (" <<
			slope << " +- " << sigmaB << ") * X" << std::endl;

		std::cout << "Intercept = " << intercept << " +- " << sigmaA << std::endl;
		std::cout << "Slope = " << slope << " +- " << sigmaB << std::endl;

		std::cout << "Lin. Error: " << uroboro::lst_sqrs_lin_error(X, Y, intercept, slope) << std::endl;

	}

	env_set_res(env, {intercept, slope});
}


void lcm_correlation_coeff(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 3) {
		std::cout << "'correlation-coeff' needs two arguments (X values, Y values)" << std::endl;
		return;
	}

	if(env.find(symbols[1]) == env.end()) {
		std::cout << "The dataset " << symbols[1] << " couldn't be found" << std::endl;
		return;
	}

	if(env.find(symbols[2]) == env.end()) {
		std::cout << "The dataset " << symbols[2] << " couldn't be found" << std::endl;
		return;
	}

	vec_buff& X = env[symbols[1]];
	vec_buff& Y = env[symbols[2]];

	real corr_coeff = uroboro::correlation_coefficient(X, Y);
	std::cout << "Correlation Coefficient: " << corr_coeff << std::endl;

	env_set_res(env, {corr_coeff});
}


void lcm_r_squared(const LCSymList& symbols, std::map<std::string, vec_buff>& env) {

	if(symbols.size() < 3) {
		std::cout << "'r-squared' needs two arguments (X values, Y values, [sigmaY])" << std::endl;
		return;
	}

	if(env.find(symbols[1]) == env.end()) {
		std::cout << "The dataset " << symbols[1] << " couldn't be found" << std::endl;
		return;
	}

	if(env.find(symbols[2]) == env.end()) {
		std::cout << "The dataset " << symbols[2] << " couldn't be found" << std::endl;
		return;
	}

	real r_squared = uroboro::square(uroboro::correlation_coefficient(env[symbols[1]], env[symbols[2]]));
	std::cout << "r-Squared: " << r_squared << std::endl;

	env_set_res(env, {r_squared});
}


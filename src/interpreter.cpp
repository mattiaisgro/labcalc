#include "interpreter.h"
#include "util.h"


LCCommandTable commtable;


void insert_command(const std::string& symbol, LCCommand command) {
	commtable.insert(std::pair<std::string, LCCommand>(symbol, command));
}


void init_commandtable() {

	// Common commands
	insert_command("quit", lc_quit);
	insert_command("insert", lc_insert);
	insert_command("load", lc_load);
	insert_command("load-as", lc_load_as);
	insert_command("save", lc_save);
	insert_command("autosave", lc_autosave);
	insert_command("help", lc_help);
	insert_command("append", lc_append);
	insert_command("prepend", lc_prepend);
	insert_command("print", lc_print);
	insert_command("print-lines", lc_print_lines);
	insert_command("exec", lc_exec);
	insert_command("echo", lc_echo);
	insert_command("echo-noline", lc_echo_noline);
	insert_command("rename", lc_rename);
	insert_command("delete", lc_delete);
	insert_command("erase-pos", lc_erase_pos);
	insert_command("erase-value", lc_erase_value);
	insert_command("erase-all-values", lc_erase_all_values);
	insert_command("find", lc_find);
	insert_command("options", lc_options);

	// Math commands
	insert_command("size", lcm_size);
	insert_command("mean", lcm_mean);
	insert_command("variance", lcm_variance);
	insert_command("variance-sample", lcm_variance_sample);
	insert_command("stdev", lcm_stdev);
	insert_command("stdev-sample", lcm_stdev_sample);
	insert_command("stdom", lcm_stdom);
	insert_command("stdom-sample", lcm_stdom_sample);
	insert_command("relative-error", lcm_relative_error);
	insert_command("linearize", lcm_linearize);
	insert_command("correlation-coeff", lcm_correlation_coeff);
	insert_command("r-squared", lcm_r_squared);

	// Compound commands
	// insert_command("stats", lcm_stats);
	// insert_command("stats-sample", lcm_stats_sample);

}


void init_options(LCEnv& env) {

	vec_buff options;

	// options[0] = output-precision
	options.emplace_back(6);
	std::cout.precision(6);

	// options[1] = silent-scripts
	options.emplace_back(0);

	// options[2] = always-overwrite
	options.emplace_back(0);

	// options[3] = color-output
	// options.emplace_back(0);

	env_insert(env, "$OPTIONS", options);
}


void interpret(const LCSymList& symbols, LCEnv& env) {

	std::string command = to_lower_case(symbols[0]);

	if(commtable.find(command) == commtable.end()) {
		std::cout << "Unknown command '" << command << "'" << std::endl;
		return;
	}

	commtable[command](symbols, env);
}

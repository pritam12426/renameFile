#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "CmakeConfig.h"
#include "functions.hpp"


namespace fs = std::filesystem;

namespace env {
	std::string user;
	fs::path home;
	fs::path pwd;
};  // namespace env

std::vector<std::string> IGNORE_PATH(
    { ".git", ".gitignore", "CMakeCache.txt", "CMakeLists.txt" });

int main(const int argc, const char *const argv[]) {
	// Get environment variables with logging
	if (char *GET_ENV = std::getenv("HOME"); GET_ENV != nullptr) {
		env::home = GET_ENV;
	} else {
		spdlog::error("HOME environment variable is not set.");
		return 1;
	}

	if (char *GET_ENV = std::getenv("USER"); GET_ENV != nullptr) {
		env::user = GET_ENV;
	} else {
		spdlog::error("USER environment variable is not set.");
		return 1;
	}

	if (char *GET_ENV = std::getenv("PWD"); GET_ENV != nullptr) {
		env::pwd = GET_ENV;
	} else {
		spdlog::error("PWD environment variable is not set.");
		return 1;
	}

	argparse::ArgumentParser program(PROJECT_NAME, PROJECT_VERSION);

	program.add_argument("--no-log", "-l")
	    .help("Disable verbose output (default is verbose)")
	    .default_value(false)
	    .implicit_value(true);

	program.add_argument("--force", "-s")
	    .help("Work with home user folder")
	    .default_value(false)
	    .implicit_value(true);

	program.add_argument("--summary", "-s")
	    .help("Printout, the summarise output")
	    .default_value(false)
	    .implicit_value(true);

	program.add_argument("--dry-run", "-n")
	    .help("Perform a trial run with no changes made")
	    .default_value(false)
	    .implicit_value(true);

	program.add_argument("--ignore", "-i")
	    .help("Ignore specific names (like build, .cache, etc.)")
	    .default_value(IGNORE_PATH)
	    .append();

	program.add_argument("files")
	    .help("List of files to rename")
	    .default_value("-=-=-=-")
	    .remaining();

	// Parse arguments
	try {
		program.parse_args(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return 1;  // Exit with error code
	}

	std::vector<std::string> files;
	try {
		files = program.get<std::vector<std::string>>("files");
	} catch (const std::bad_any_cast &e) {
		std::cerr << "Error: Unexpected argument type." << std::endl;
        std::cerr << "       " << e.what() << std::endl;
		std::cerr << "==============================\n";
		std::cerr << program.help().str();
		return 1;
	}

	// Now merge defaults + user input
	{
		auto user_ignores = program.get<std::vector<std::string>>("--ignore");
		IGNORE_PATH.insert(IGNORE_PATH.end(), user_ignores.begin(), user_ignores.end());
	}


	for (const auto &file : files) {
		fs::path const original_abs_path(fs::absolute(file));

		if (!fs::exists(original_abs_path)) {
			spdlog::warn("File not exist \"{}\"", original_abs_path.filename().c_str());
			continue;
		}

		if (original_abs_path.filename().string().starts_with(".")) {
			spdlog::warn("Not meant for working with hidden files and folder :: \"{}\"", original_abs_path.filename().c_str());
			continue;
		}

		if (original_abs_path.parent_path() == env::home) {
			if (!program.get<bool>("--force")) {
				spdlog::warn("Skipping: Part of home directory :: \"{}\"", original_abs_path.filename().c_str());
				continue;
			}
		}

		fs::path original_dir_name(original_abs_path.parent_path());
		fs::path original_file_name(original_abs_path.filename());
		fs::path new_file_name = original_file_name;

		bool ignore = false;
		for (const auto &i : IGNORE_PATH) {
			if (original_file_name == i) {
				spdlog::warn("Ignore this file \"{}\"", original_file_name.filename().c_str());
				ignore = true;
				break;
			}
		}
		if (ignore) {
			continue;
		}

		// Done till here

		if (Fun::formatPathName(new_file_name)) {
			// TODO: get back the file Extensions
			fs::path new_abs_path = original_dir_name / new_file_name;

			if (original_abs_path == new_abs_path) continue;

			if (program.get<bool>("--dry-run")) {
				spdlog::info("[DRY] Would rename \"{}\" -> \"{}\"", original_abs_path.filename().c_str(), new_abs_path.filename().c_str());
			} else {
				// fs::rename(original_abs_path, new_abs_path);
				if (!program.get<bool>("--no-log")) {
					spdlog::info("Renamed \"{}\" -> \"{}\"", original_abs_path.filename().c_str(), new_abs_path.filename().c_str());
				}
			}

			// TODO: add logoc for add data for summary
		}
	}

	if (program.get<bool>("--summary")) {
		//
	}

	return 0;
}

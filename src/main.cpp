#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>
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

std::vector<std::string> IGNORE_PATH({
	".git",
    ".gitignore",
    "CMakeCache.txt",
    "CMakeLists.txt"
});

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

	program.add_argument("--no-verbose", "-V")
	    .help("Disable verbose output (default is verbose)")
	    .default_value(false)
	    .implicit_value(true);

	program.add_argument("--force", "-F")
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
	    .remaining();

	// Parse arguments
	try {
		program.parse_args(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return 1;  // Exit with error code
	}

	// Now merge defaults + user input
	{
		auto user_ignores = program.get<std::vector<std::string>>("--ignore");
		IGNORE_PATH.insert(IGNORE_PATH.end(),
		                   user_ignores.begin(),
		                   user_ignores.end());
	}

	struct RenameLog {
		fs::path original_path;
		fs::path new_path;
	};

	std::vector<RenameLog> rename_logs;

	auto files = program.get<std::vector<std::string>>("files");

	for (const auto &file : files) {
		fs::path const original_abs_path(fs::absolute(file));

		if (!fs::exists(original_abs_path)) {
			spdlog::warn("File not exist ", original_abs_path.filename().c_str());
			continue;
		}

		if (original_abs_path.parent_path() == env::home) {
			if (!program.get<bool>("--force")) {
				spdlog::info("This is home dir {} ", original_abs_path.parent_path().c_str());
				continue;
			}
		}

		fs::path original_dir_name(original_abs_path.parent_path());
		fs::path original_file_name(original_abs_path.filename());
		fs::path new_file_name = original_file_name;

		bool ignore = false;
		for (const auto &i : IGNORE_PATH) {
			if (original_file_name == i) {
				spdlog::warn("Ignore this file {} ", original_file_name.c_str());
				ignore = true;
				break;
			}
		}
		if (ignore) {
			continue;
		}

		if (Fun::formatPathName(new_file_name)) {
			fs::path new_abs_path = original_dir_name / new_file_name;

			if (program.get<bool>("--dry-run")) {
				spdlog::info("[DRY] Would rename {} -> {}", original_abs_path.c_str(),new_abs_path.c_str());
				if (original_abs_path != new_abs_path) {
					rename_logs.push_back({original_abs_path, new_abs_path});
				}
			} else {
				if (original_abs_path != new_abs_path) {
					fs::rename(original_abs_path, new_abs_path.c_str());
					if (!program.get<bool>("--no-verbose"))
						spdlog::info("Renamed {} -> {}", original_abs_path.c_str(), new_abs_path.c_str());
					rename_logs.push_back({original_abs_path, new_abs_path});
				}
			}
		}
	}

	if (program.get<bool>("--summary")) {
		if (rename_logs.empty()) {
			spdlog::info("No files were renamed.");
		} else {
			spdlog::info("Summary of renamed files:");
			for (const auto &log : rename_logs) {
				spdlog::info("  {} -> {}", log.original_path.c_str(), log.new_path.c_str());
			}
		}
	}

	return 0;
}

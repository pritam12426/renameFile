#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>

#include "CmakeConfig.h"
#include "functions.hpp"


namespace fs = std::filesystem;

namespace env {
	std::string user;
	fs::path home;
	fs::path pwd;
};  // namespace env

int main(const int argc, const char *const argv[]) {
	argparse::ArgumentParser program(PROJECT_NAME, PROJECT_VERSION);

	env::home = std::getenv("HOME");
	env::user = std::getenv("USER");
	env::pwd  = std::getenv("PWD");

	for (int i = 1; i < argc; i++) {
		fs::path abs_path(fs::absolute(argv[i]));

		if (!fs::exists(abs_path)) {
			spdlog::error("This is home dir {} ", abs_path.filename().c_str());
			continue;
		}

		if (abs_path.parent_path() == env::home) {
			// if (log) {
			spdlog::error("This is home dir {} ", abs_path.filename().c_str());
			// }
			continue;
		}

		if (fs::is_regular_file(abs_path)) {
			std::cout << "It is a file\n";
		} else if (fs::is_directory(abs_path)) {
			std::cout << "It is a directory\n";
		}
	}

	return 0;
}

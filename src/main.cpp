#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "CmakeConfig.h"
#include "functions.hpp"

namespace fs = std::filesystem;

namespace env {
	std::string user;
	std::string home;
	std::string pwd;
};  // namespace env

int main(const int argc, const char *const argv[]) {
	argparse::ArgumentParser program(PROJECT_NAME, PROJECT_VERSION);

	env::home = std::getenv("HOME");
	env::user = std::getenv("USER");
	env::pwd  = std::getenv("PWD");

	for (int i = 1; i < argc; i++) {
		fs::path original(argv[i]);
		std::cout << original << "\n\n\n";
	}


	std::printf("Your prject \"%s\" --version \"%s\". Dis-[ %s ] is ready "
	            "to.\nURL -- %s\n",
	            PROJECT_NAME,
	            PROJECT_VERSION,
	            PROJECT_DESCRIPTION,
	            PROJECT_HOMEPAGE_URL);
}

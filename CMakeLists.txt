cmake_minimum_required(VERSION 4.0.0)

project(
  "Fix File And Folder Name"
  VERSION 0.1.0
  DESCRIPTION "A tool to rename files in a directory based on specific patterns."
  HOMEPAGE_URL "https://github.com/pritam12426/"
  LANGUAGES CXX
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/src/CmakeConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/CmakeConfig.h)

#### C++ Standard Settings ####
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MAIN_BINARY "fix-name")

#### Source Files ####
file(GLOB SRC_FILES
  "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
)

list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

#### Dependencies ####
# find_package(PkgConfig REQUIRED)
find_package(argparse REQUIRED)
find_package(spdlog REQUIRED)
# pkg_check_modules(LIBXML REQUIRED libxml++-5.0)

#### Main Executable ####
add_executable(${MAIN_BINARY}
  ${SRC_FILES}
  "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
)

target_link_libraries(${MAIN_BINARY} PRIVATE
  argparse::argparse
  spdlog::spdlog
  # ${<Pkg>_LDFLAGS}       # only if pkg-config used
  # ${<Pkg>_LIBRARIES}
)

target_include_directories(${MAIN_BINARY} PRIVATE
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}
  # ${<Pkg>_INCLUDE_DIRS}
)

add_custom_target(run
  COMMAND ${MAIN_BINARY}
  COMMENT "Runing ${MAIN_BINARY}"
  DEPENDS ${MAIN_BINARY}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

#### Setting the conditional variable ####
option(BUILD_TESTING "Enable unit testing"           OFF)
option(BUILD_DOC     "Generate doc for the project"  OFF)

#### Unit Tests ####
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

#### Bulding doc ####
if(BUILD_DOC)
  add_subdirectory(doc)
endif()

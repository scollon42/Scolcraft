cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name, my project isn't very descriptive
project(game CXX)
include(cmake/StandardProjectSettings.cmake)
# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which
    # is a good idea if any #define's change
    target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
endif ()

option(ENABLE_COTIRE "Enable cotire to speed up builds" ON)
if (ENABLE_COTIRE)
    include(cmake/cotire.cmake)
endif ()


# Set up some extra Conan dependencies based on our needs
# before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
        glad/0.1.33
        glfw/3.3.2@bincrafters/stable
        glm/0.9.9.8
        stb/20190512@conan/stable
        )

set(CONAN_EXTRA_OPTIONS ${CONAN_EXTRA_OPTIONS}
        )

include(cmake/Conan.cmake)
run_conan()

if (ENABLE_TESTING)
    enable_testing()
    message(
            "Building Tests. Be sure to check out test/constexpr_tests for constexpr testing"
    )
    add_subdirectory(test)
endif ()

add_subdirectory(src)

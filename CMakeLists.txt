cmake_minimum_required(VERSION 3.26)

project(RaycasterProject VERSION 1.0 DESCRIPTION "Raycaster Game" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the output directories for build files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.0
)

FetchContent_GetProperties(sfml)
if(NOT sfml_POPULATED)
  FetchContent_Populate(sfml)
  add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif()

# Glob for source and header files in the src directory
file(GLOB_RECURSE RAYCASTER_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE RAYCASTER_HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)

# Add executable target including the main.cpp in the root directory
add_executable(${PROJECT_NAME} main.cpp ${RAYCASTER_SOURCES} ${RAYCASTER_HEADERS})

# Organize sources into groups for IDEs
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${RAYCASTER_SOURCES})
source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${RAYCASTER_HEADERS})

# Enable warnings and other compilation options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /Zc:preprocessor /Zc:__cplusplus)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-system)

# Installation rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

# CPack configuration
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "RaycasterProject")
set(CPACK_PACKAGE_VENDOR "Darleanow")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Raycaster - A fast-paced 2D Raycasting game.")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "RaycasterProject ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/Readme.md")

# Specify the generator
set(CPACK_GENERATOR "NSIS")

include(CPack)

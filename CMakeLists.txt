cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(btag)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER MATCHES clang\\+\\+)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -O0")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER MATCHES clang\\+\\+)

find_package(Boost 1.46.0 COMPONENTS filesystem system REQUIRED)

find_package(PkgConfig)
pkg_check_modules(TAGLIB REQUIRED taglib)

set(BTAG_SOURCES
    src/BasicStringFilter.cpp
    src/ConfirmationHandler.cpp
    src/EnglishTitleLocalizationHandler.cpp
    src/InteractiveTagger.cpp
    src/main.cpp
    src/RenamingFilter.cpp
    src/SimpleCapitalizationFilter.cpp
    src/SpanishTitleLocalizationHandler.cpp
    src/StandardConsole.cpp
    src/TitleCapitalizationFilter.cpp
    src/TitleLocalizationHandler.cpp)

set(BTAG_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/src/config.h
    src/BasicStringFilter.h
    src/CapitalizationFilter.h
    src/ConfirmationHandler.h
    src/ConservativeRenamingFilter.h
    src/EnglishTitleLocalizationHandler.h
    src/InteractiveTagger.h
    src/RenamingFilter.h
    src/SimpleCapitalizationFilter.h
    src/SpanishTitleLocalizationHandler.h
    src/StandardConsole.h
    src/TitleCapitalizationFilter.h
    src/TitleLocalizationHandler.h
    src/UnixRenamingFilter.h
    src/wide_string_cast.h)

option(CUEFILE_SUPPORT "Support for reading metadata from CUE files (requires libcue)" ON)
if(CUEFILE_SUPPORT)
    pkg_check_modules(LIBCUE REQUIRED libcue)
    set(BTAG_SOURCES ${BTAG_SOURCES} src/CueReader.cpp)
    set(BTAG_HEADERS ${BTAG_HEADERS} src/CueReader.h)
endif(CUEFILE_SUPPORT)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/config.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${Boost_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
    ${LIBCUE_INCLUDE_DIRS})

add_executable(btag ${BTAG_SOURCES} ${BTAG_HEADERS})
target_link_libraries(btag ${Boost_LIBRARIES} ${TAGLIB_LIBRARIES} ${LIBCUE_LIBRARIES})

install(TARGETS btag DESTINATION bin)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "/.git;/.gitignore;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

option(ENABLE_TESTS "Whether a \"make check\" target should be made available" OFF)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_TESTS)

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(btag)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

list(APPEND CMAKE_PREFIX_PATH /usr/local)
find_package(Boost 1.49.0 COMPONENTS filesystem locale system REQUIRED)

find_package(TagLib REQUIRED)
find_package(LibEdit REQUIRED)

option(CUESHEET_SUPPORT "Support for reading metadata from cue sheets" ON)
if(CUESHEET_SUPPORT)
    find_package(LibCue REQUIRED)
    find_package(Iconv REQUIRED)
    set(EXTRA_LIBRARIES ${EXTRA_LIBRARIES} ${LIBCUE_LIBRARIES} ${ICONV_LIBRARIES})
endif(CUESHEET_SUPPORT)

set(PROGRAM_NAME ${PROJECT_NAME})

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
    src/number_cast.h
    src/RenamingFilter.h
    src/SimpleCapitalizationFilter.h
    src/SpanishTitleLocalizationHandler.h
    src/StandardConsole.h
    src/TitleCapitalizationFilter.h
    src/TitleLocalizationHandler.h
    src/UnixRenamingFilter.h
    src/wide_string_cast.h)

if(CUESHEET_SUPPORT)
    set(BTAG_SOURCES
        ${BTAG_SOURCES}
        src/CueReader.cpp
        src/CueReaderMultiplexer.cpp)
    set(BTAG_HEADERS
        ${BTAG_HEADERS}
        src/CueReader.h
        src/CueReaderMultiplexer.h)
endif(CUESHEET_SUPPORT)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/config.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/src
    ${Boost_INCLUDE_DIRS}
    ${LIBCUE_INCLUDE_DIRS}
    ${LIBEDIT_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS})

add_executable(btag ${BTAG_SOURCES} ${BTAG_HEADERS})
target_link_libraries(btag
    ${Boost_LIBRARIES}
    ${LIBEDIT_LIBRARIES}
    ${TAGLIB_LIBRARIES}
    ${EXTRA_LIBRARIES})

install(TARGETS btag DESTINATION bin)

option(ENABLE_TESTS "Make the check target available" OFF)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_TESTS)

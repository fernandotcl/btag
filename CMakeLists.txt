cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(btag)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -O0")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package(Boost 1.42.0 COMPONENTS filesystem REQUIRED)

find_package(PkgConfig)
pkg_check_modules(TAGLIB REQUIRED taglib)

set(BTAG_SOURCES
    src/BasicStringFilter.cpp
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
    src/BasicStringFilter.h
    src/CapitalizationFilter.h
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

include_directories(${Boost_INCLUDE_DIRS} ${TAGLIB_INCLUDE_DIRS})

add_executable(btag ${BTAG_SOURCES} ${BTAG_HEADERS})
target_link_libraries(btag ${Boost_LIBRARIES} ${TAGLIB_LIBRARIES})

install(TARGETS btag DESTINATION bin)

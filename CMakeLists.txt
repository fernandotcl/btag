cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(qtagger)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -ggdb -O0 -D_DEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package(Boost 1.42.0 COMPONENTS filesystem REQUIRED)

find_package(PkgConfig)
pkg_check_modules(TAGLIB REQUIRED taglib)

set(QTAGGER_SOURCES
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

set(QTAGGER_HEADERS
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

add_executable(qtagger ${QTAGGER_SOURCES} ${QTAGGER_HEADERS})
target_link_libraries(qtagger ${Boost_LIBRARIES} ${TAGLIB_LIBRARIES})

add_custom_target(dist
    COMMAND nroff -t man man/qtagger.1.roff >man/qtagger.1
    COMMAND git describe --tags --abbrev=0 >version
    COMMAND git archive --prefix=qtagger-`cat version`/ HEAD >qtagger-`cat version`.tar
    COMMAND tar --transform s@^@qtagger-`cat version`/@ -rf qtagger-`cat version`.tar man/qtagger.1
    COMMAND gzip qtagger-`cat version`.tar
    COMMAND mv qtagger-`cat version`.tar.gz ${CMAKE_BINARY_DIR}
    COMMAND rm -f version man/qtagger.1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(TARGETS qtagger DESTINATION bin)

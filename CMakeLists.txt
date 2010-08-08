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
    src/InteractiveTagger.cpp
    src/main.cpp
    src/StandardConsole.cpp)

set(QTAGGER_HEADERS
    src/BasicStringFilter.h
    src/InteractiveTagger.h
    src/StandardConsole.h)

include_directories(${Boost_INCLUDE_DIRS} ${TAGLIB_INCLUDE_DIRS})

add_executable(qtagger ${QTAGGER_SOURCES} ${QTAGGER_HEADERS})
target_link_libraries(qtagger ${Boost_LIBRARIES} ${TAGLIB_LIBRARIES})

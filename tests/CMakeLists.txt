include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

add_executable(
    english_titles_test
    english_titles.cpp
    titles_base.cpp
    titles_base.h
    ${PROJECT_SOURCE_DIR}/src/BasicStringFilter.cpp
    ${PROJECT_SOURCE_DIR}/src/BasicStringFilter.h
    ${PROJECT_SOURCE_DIR}/src/CapitalizationFilter.h
    ${PROJECT_SOURCE_DIR}/src/EnglishTitleLocalizationHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/EnglishTitleLocalizationHandler.h
    ${PROJECT_SOURCE_DIR}/src/TitleCapitalizationFilter.cpp
    ${PROJECT_SOURCE_DIR}/src/TitleCapitalizationFilter.h
    ${PROJECT_SOURCE_DIR}/src/TitleLocalizationHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/TitleLocalizationHandler.h
)
target_link_libraries(english_titles_test ${Boost_LIBRARIES})
add_test(english_titles_test
    ${PROJECT_BINARY_DIR}/tests/english_titles_test
    ${PROJECT_SOURCE_DIR}/tests/data/english_titles.txt)

add_executable(
    spanish_titles_test
    spanish_titles.cpp
    titles_base.cpp
    titles_base.h
    ${PROJECT_SOURCE_DIR}/src/BasicStringFilter.cpp
    ${PROJECT_SOURCE_DIR}/src/BasicStringFilter.h
    ${PROJECT_SOURCE_DIR}/src/CapitalizationFilter.h
    ${PROJECT_SOURCE_DIR}/src/SpanishTitleLocalizationHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/SpanishTitleLocalizationHandler.h
    ${PROJECT_SOURCE_DIR}/src/TitleCapitalizationFilter.cpp
    ${PROJECT_SOURCE_DIR}/src/TitleCapitalizationFilter.h
    ${PROJECT_SOURCE_DIR}/src/TitleLocalizationHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/TitleLocalizationHandler.h
)
target_link_libraries(spanish_titles_test ${Boost_LIBRARIES})
add_test(spanish_titles_test
    ${PROJECT_BINARY_DIR}/tests/spanish_titles_test
    ${PROJECT_SOURCE_DIR}/tests/data/spanish_titles.txt)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

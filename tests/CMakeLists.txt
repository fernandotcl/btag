include_directories(${Boost_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)

add_executable(
    EnglishTitleCapitalizationTest
    EnglishTitleCapitalization.cpp
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
target_link_libraries(EnglishTitleCapitalizationTest ${Boost_LIBRARIES})
add_test(EnglishTitleCapitalizationTest ${PROJECT_BINARY_DIR}/tests/EnglishTitleCapitalizationTest)

add_executable(
    SpanishTitleCapitalizationTest
    SpanishTitleCapitalization.cpp
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
target_link_libraries(SpanishTitleCapitalizationTest ${Boost_LIBRARIES})
add_test(SpanishTitleCapitalizationTest
    ${PROJECT_BINARY_DIR}/tests/SpanishTitleCapitalizationTest
    ${PROJECT_SOURCE_DIR}/tests/data/spanish_titles.txt)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

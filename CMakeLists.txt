cmake_minimum_required(VERSION 3.24)

project(Turing
  LANGUAGES CXX
  VERSION 0.2
)

set(CMAKE_BUILD_TYPE Release)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.0.0
  FIND_PACKAGE_ARGS NAMES ftxui
)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.10.5
    FIND_PACKAGE_ARGS NAMES nlohmann_json
)

FetchContent_MakeAvailable(ftxui json)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)

add_library(TuringLib src/turing.cpp)
add_library(TuringUI src/turingUI.cpp)
add_library(Translate src/translate.cpp)

target_link_libraries(Translate TuringLib)

target_link_libraries(TuringUI PRIVATE Translate ftxui::screen ftxui::dom ftxui::component TuringLib nlohmann_json::nlohmann_json)

add_executable(turing-cmd src/main.cpp)
target_link_libraries(turing-cmd PRIVATE Translate ftxui::screen ftxui::dom ftxui::component TuringLib TuringUI nlohmann_json::nlohmann_json)

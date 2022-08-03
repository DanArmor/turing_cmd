cmake_minimum_required(VERSION 3.21)

project(Turing
  LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v2.0.0
)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.10.5
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
#include_directories(${json_SOURCE_DIR}/include)

add_library(TuringLib src/turing.cpp)
add_library(TuringUI src/turingUI.cpp)
#add_library(Utils src/utils.cpp)

target_link_libraries(TuringUI PRIVATE ftxui::screen ftxui::dom ftxui::component TuringLib nlohmann_json::nlohmann_json)

#target_link_libraries(Utils PRIVATE nlohmann_json::nlohmann_json)

add_executable(MAIN src/main.cpp)

target_link_libraries(MAIN PRIVATE ftxui::screen ftxui::dom ftxui::component TuringLib TuringUI nlohmann_json::nlohmann_json)
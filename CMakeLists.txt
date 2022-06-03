cmake_minimum_required(VERSION 3.21)
project(Turing)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v2.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
#include_directories(${ftxui_SOURCE_DIR}/include)

add_library(TuringLib src/turing.cpp)
add_library(TuringUI src/turingUI.cpp)
target_link_libraries(TuringUI PRIVATE ftxui::screen ftxui::dom ftxui::component TuringLib)

add_executable(MAIN src/main.cpp)

target_link_libraries(MAIN PRIVATE ftxui::screen ftxui::dom ftxui::component TuringLib TuringUI)
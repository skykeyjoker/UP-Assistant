cmake_minimum_required(VERSION 3.20)
project(UP_Assistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${CMAKE_CURRENT_LIST_DIR}/3rd)

find_package(Qt5 COMPONENTS Widgets Core Network)

add_executable(UP_Assistant main.cpp UpAssistantWidget.cpp UpAssistantWidget.h 3rd/json.hpp BVideo.cpp BVideo.h BiliBiliQuery.cpp BiliBiliQuery.h BiliBiliCard.cpp BiliBiliCard.h)

target_link_libraries(UP_Assistant Qt5::Widgets Qt5::Network Qt5::Core)

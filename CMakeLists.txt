cmake_minimum_required(VERSION 3.20)
project(UP-Assistant)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${CMAKE_CURRENT_LIST_DIR}/3rd)

find_package(Qt5 COMPONENTS Widgets Core Network)

if (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
    link_libraries(${CMAKE_CURRENT_LIST_DIR}/3rd/QXlsx/lib/QXlsxd.lib)
elseif (CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
    link_libraries(${CMAKE_CURRENT_LIST_DIR}/3rd/QXlsx/lib/QXlsx.lib)
else ()
    message("else:${CMAKE_BUILD_TYPE}")
    message("else:${CMAKE_C_FLAGS_RELEASE}")
    link_libraries(${CMAKE_CURRENT_LIST_DIR}/3rd/QXlsx/lib/QXlsx.lib)
endif ()

qt5_add_resources(QRC_FILES res.qrc)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/logo.rc")

add_executable(UP-Assistant WIN32 ${QRC_FILES} ${APP_ICON_RESOURCE_WINDOWS} main.cpp UpAssistantWidget.cpp UpAssistantWidget.h 3rd/json.hpp BVideo.cpp BVideo.h BiliBiliQuery.cpp BiliBiliQuery.h BiliBiliCard.cpp BiliBiliCard.h NewTaskDialog.cpp NewTaskDialog.h)

target_link_libraries(UP-Assistant Qt5::Widgets Qt5::Network Qt5::Core)

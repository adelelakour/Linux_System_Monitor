cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)

add_executable(monitor
        src/main.cpp
        src/util.cpp
        src/ProcessParser.cpp
        src/SysInfo.cpp
        src/process.cpp
        src/system.cpp
        src/format.cpp
        src/processor.cpp
        src/ProcessContainer.cpp
)

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

cmake_minimum_required(VERSION 3.6.2)
project(specformatter)

#Use C++ 11
set(CMAKE_CXX_STANDARD 11)

#Export compile commands for editor autocomplete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Be really pedantic!
if(MSVC)
    add_definitions(/W4)
else()
    add_definitions(-Wall -Wextra -pedantic)
endif()

#Automatically run moc, uic and rcc when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#External dependencies
find_package(Qt5Core REQUIRED)

include_directories(./include)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCE_FILES
    ./src/main.cpp
    ./src/SpecFormatter.cpp
    ./src/Spec.cpp
    ./src/Section.cpp
    ./src/Entry.cpp
    ./src/Flag.cpp
    ./src/Note.cpp
    )

set(HEADER_FILES
    ./include/SpecFormatter.hpp
    ./include/Spec.hpp
    ./include/Section.hpp
    ./include/Entry.hpp
    ./include/Flag.hpp
    ./include/Note.hpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

#External dependencies
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    )

cmake_minimum_required(VERSION 3.19) # TODO check this one (I think the version will not matter with what I use)
project(Wonderland)
# TODO_Future support other compilers (now only supported with MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

option(SFML_PATH "Path to SFML library package" "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1")
option(IRRKLANG_PATH "Path to IRRKLANG library package" "${PROJECT_SOURCE_DIR}/modules/irrKlang-64bit-1.6.0")

file(GLOB WONDERLAND_HEADERS "${PROJECT_SOURCE_DIR}/inc/*")
file(GLOB WONDERLAND_SRC "${PROJECT_SOURCE_DIR}/src/*")

# Visual studio for some reason does not recognize the default option() value
if (NOT SFML_PATH)
    set(SFML_PATH "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1")
endif()

if (NOT IRRKLANG_PATH)
    set(IRRKLANG_PATH "${PROJECT_SOURCE_DIR}/modules/irrKlang-64bit-1.6.0")
endif()

message(STATUS "HERe is the list ${ALL_LIB_FILES_IN_SFML}")
add_executable(main "${WONDERLAND_HEADERS}" "${WONDERLAND_SRC}" main.cpp)

# TODO link with sfml and check whether it works
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/inc")
target_include_directories(main PUBLIC "${SFML_PATH}/include")
target_include_directories(main PUBLIC "${IRRKLANG_PATH}/include")
message(STATUS "${SFML_PATH}")
# TODO only add required lib files
# Add all files which end with .lib (BAD SOLUTION BUT now just want it to work)
file(GLOB ALL_SFML_DEBUG_LIB
"${SFML_PATH}/lib/*-d.lib"
)

file(GLOB ALL_SFML_Release_LIB
"${SFML_PATH}/lib/*.lib"
)

# filter the debug ones from the release list
list(FILTER ALL_SFML_Release_LIB EXCLUDE REGEX ".*\-d.lib$")
list(FILTER ALL_SFML_Release_LIB EXCLUDE REGEX ".*\-s.lib$")

target_link_libraries(main PUBLIC 
"$<$<CONFIG:Debug>:${ALL_SFML_DEBUG_LIB}>"
"$<$<CONFIG:Release>:${ALL_SFML_Release_LIB}>"
"${IRRKLANG_PATH}/lib/Winx64-visualStudio/irrKlang.lib")

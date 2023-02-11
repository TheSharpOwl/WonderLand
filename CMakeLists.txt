cmake_minimum_required(VERSION 3.19) # TODO check this one (I think the version will not matter with what I use)
project(Wonderland)
# TODO_Future support other compilers (now only supported with MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

option(SFML_PATH "Path to SFML library package" "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1")

file(GLOB WONDERLAND_HEADERS "${PROJECT_SOURCE_DIR}/inc/*")
file(GLOB WONDERLAND_SRC "${PROJECT_SOURCE_DIR}/src/*")

# Visual studio for some reason does not recognize the default option() value
if (NOT SFML_PATH)
    set(SFML_PATH "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1")
endif()
message(STATUS "HERe is the list ${ALL_LIB_FILES_IN_SFML}")
add_executable(main "${WONDERLAND_HEADERS}" "${WONDERLAND_SRC}" main.cpp)

# TODO link with sfml and check whether it works
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/inc")
target_include_directories(main PUBLIC "${SFML_PATH}/include")
message(STATUS "${SFML_PATH}")
# TODO only add required lib files
# Add all files which end with .lib (BAD SOLUTION BUT now just want it to work)
file(GLOB ALL_LIB_FILES_IN_SFML
     "${SFML_PATH}/lib/*.lib"
)

target_link_libraries(main PUBLIC "${ALL_LIB_FILES_IN_SFML}")

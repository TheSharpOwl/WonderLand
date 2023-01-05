cmake_minimum_required(VERSION 3.19) # TODO check this one (I think the version will not matter with what I use)
project(Wonderland)

# TODO_Future support other compilers (now only supported with MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

add_executable(main main.cpp)

# TODO link with sfml and check whether it works
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

# TODO use cmake variable this is temporary
target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1/include")

# TODO only add required lib files
# Add all files which end with .lib (BAD SOLUTION BUT now just want it to work)
file(GLOB ALL_LIB_FILES_IN_SFML
     "${PROJECT_SOURCE_DIR}/modules/SFML-2.5.1/lib/*.lib"
)

target_link_libraries(main PUBLIC "${ALL_LIB_FILES_IN_SFML}")

# TODO remove this line
# To make sure they are added debugging 
message(STATUS "HERe is the list ${ALL_LIB_FILES_IN_SFML}")

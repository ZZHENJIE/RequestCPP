cmake_minimum_required(VERSION 3.12)

project(Curl)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

link_libraries(-lcurl.dll)

add_executable(${PROJECT_NAME}
    src/Request.cpp
    src/Example.cpp
)
cmake_minimum_required(VERSION 3.22)
project(15)

set(CMAKE_CXX_STANDARD 17)
include_directories("/opt/homebrew/Cellar/sfml/2.5.1_1/include")
file(GLOB LIBRARIES "/opt/homebrew/Cellar/sfml/2.5.1_1/lib/*.dylib")
message("LIBRARIES = ${LIBRARIES}")

add_executable(${PROJECT_NAME}
        main.cpp settings.h Button.cpp Button.h Game.cpp Game.h)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})



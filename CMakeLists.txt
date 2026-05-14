cmake_minimum_required(VERSION 3.20)
project(Battleship VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Battleship ${SOURCES})

target_link_libraries(Battleship PRIVATE raylib)
target_include_directories(Battleship PRIVATE src)

if (EMSCRIPTEN)
    # Set Emscripten flags
    set_target_properties(Battleship PROPERTIES SUFFIX ".html")
    target_link_options(Battleship PRIVATE -s USE_GLFW=3 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --preload-file assets)
elseif (WIN32)
    # Statically link MinGW runtime so the .exe can run on any Windows PC without DLLs
    target_link_options(Battleship PRIVATE -static -static-libgcc -static-libstdc++)
endif()

cmake_minimum_required(VERSION 3.15)
project(tetris)

# 设置vcpkg工具链
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
set(SFML_DIR "D:/vcpkg/installed/x64-mingw-dynamic/share/sfml")

# 生成compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS_OUTPUT_DIR ${CMAKE_SOURCE_DIR})

# 指定C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 可执行文件
add_executable(tetris 
    src/main.cpp
)

# 查找并链接SFML
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
target_link_libraries(tetris PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

#自定义编译命令
target_compile_options(tetris PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -fno-common
)

# 复制SFML的dll
if(WIN32)
    add_custom_command(TARGET tetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/vcpkg/installed/x64-mingw-dynamic/bin/sfml-graphics-3.dll"
        "$<TARGET_FILE_DIR:tetris>"
    )
    add_custom_command(TARGET tetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/vcpkg/installed/x64-mingw-dynamic/bin/sfml-window-3.dll"
        "$<TARGET_FILE_DIR:tetris>"
    )
    add_custom_command(TARGET tetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/vcpkg/installed/x64-mingw-dynamic/bin/sfml-system-3.dll"
        "$<TARGET_FILE_DIR:tetris>"
    )
endif()
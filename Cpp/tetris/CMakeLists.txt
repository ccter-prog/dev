cmake_minimum_required(VERSION 3.15)
project(tetris)

# 设置 compile_commands.json 生成在源码目录
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# set(CMAKE_EXPORT_COMPILE_COMMANDS_OUTPUT_DIR ${CMAKE_SOURCE_DIR})

# 平台特定的工具链配置
if(WIN32)
    # Windows: 使用 vcpkg
    set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic")
    set(SFML_DIR "D:/vcpkg/installed/x64-mingw-dynamic/share/sfml")
elseif(APPLE)
    # macOS: 尝试检测 Brew libc++
    find_library(BREW_LIBCXX NAMES libc++.dylib libc++.a
        PATHS 
            /usr/local/opt/llvm/lib
            /opt/homebrew/opt/llvm/lib
            /usr/local/lib
        NO_DEFAULT_PATH
    )
    
    if(BREW_LIBCXX)
        message(STATUS "Found Brew libc++: ${BREW_LIBCXX}")
        # 优先使用 Brew 的编译器
        if(EXISTS "/usr/local/opt/llvm/bin/clang++")
            set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
            set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
        elseif(EXISTS "/opt/homebrew/opt/llvm/bin/clang++")
            set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
            set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
        endif()
    else()
        message(STATUS "Brew libc++ not found, using system libc++")
    endif()
    
    # macOS 框架依赖 - 先查找但不立即链接
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
endif()

# 通用配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tetris src/main.cpp)

# 查找 SFML（跨平台）
find_package(SFML COMPONENTS Graphics Window System REQUIRED)
target_link_libraries(tetris PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

# 平台特定的目标设置
if(APPLE)
    # macOS 框架链接
    if(COCOA_LIBRARY AND IOKIT_LIBRARY)
        target_link_libraries(tetris PRIVATE 
            ${COCOA_LIBRARY} 
            ${IOKIT_LIBRARY}
        )
    endif()
    
    # 使用 libc++（如果找到了 Brew 的版本）
    if(BREW_LIBCXX)
        target_compile_options(tetris PRIVATE -stdlib=libc++)
        target_link_options(tetris PRIVATE -stdlib=libc++)
    endif()
endif()

# 通用编译警告选项
target_compile_options(tetris PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -fno-common
    $<$<CXX_COMPILER_ID:Clang>:
        -Wconversion
        -Wsign-conversion
    >
)

# Windows 特定的 DLL 复制
if(WIN32 AND NOT VCPKG_TARGET_IS_STATIC)
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

# 配置信息显示
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
if(APPLE)
    if(BREW_LIBCXX)
        message(STATUS "Using: Brew libc++")
    else()
        message(STATUS "Using: System libc++")
    endif()
endif()
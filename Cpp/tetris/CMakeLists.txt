cmake_minimum_required(VERSION 3.15)
project(tetris)

# 通用配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置 compile_commands.json 生成在源码目录
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 查找 SFML（跨平台）
find_package(SFML 3.0.2 COMPONENTS Graphics Window System Audio REQUIRED)

# 创建可执行文件
add_executable(tetris src/main.cpp src/Tetromino.cpp src/Board.cpp src/Game.cpp)

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

# 链接SFML库
target_link_libraries(tetris SFML::Graphics SFML::Window SFML::System SFML::Audio)

# 平台特定的工具链配置
if(WIN32)
    # 复制 SFML DLL
    foreach(module Graphics Window System Audio)
        if(TARGET SFML::${module})
            add_custom_command(
                TARGET tetris POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SFML::${module}>"
                $<TARGET_FILE_DIR:tetris>
                COMMENT "Copying SFML ${module} DLL"
            )
        endif()
    endforeach()

    # 根据构建类型选择 DLL 目录和文件名
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(VCPKG_BIN_DIR "D:/vcpkg/installed/x64-mingw-dynamic/debug/bin")
        set(SFML_DEPENDENCY_DLLS
            "libfreetyped.dll"
            "libpng16d.dll"
            "libzlibd1.dll"
            "libbrotlicommon.dll"
            "libbrotlidec.dll"
            "libbz2d.dll"
            "libvorbisfiled.dll"
            "libvorbisencd.dll"
            "libvorbisd.dll"
            "libFLACd.dll"
            "liboggd.dll"
        )
    else()
        set(VCPKG_BIN_DIR "D:/vcpkg/installed/x64-mingw-dynamic/bin")
        set(SFML_DEPENDENCY_DLLS
            "libfreetype.dll"
            "libpng16.dll"
            "libzlib1.dll"
            "libbrotlicommon.dll"
            "libbrotlidec.dll"
            "libbz2.dll"
            "libvorbisfile.dll"
            "libvorbisenc.dll"
            "libvorbis.dll"
            "libFLAC.dll"
            "libogg.dll"
        )
    endif()

    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "Using VCPKG directory: ${VCPKG_BIN_DIR}")

    # 复制依赖 DLL
    foreach(dll_name ${SFML_DEPENDENCY_DLLS})
        set(dll_path "${VCPKG_BIN_DIR}/${dll_name}")
        message(STATUS "Checking DLL: ${dll_path}")
        if(EXISTS "${dll_path}")
            message(STATUS "Found dependency DLL: ${dll_name}")
            add_custom_command(
                TARGET tetris POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${dll_path}"
                    $<TARGET_FILE_DIR:tetris>
                COMMENT "Copying ${dll_name}"
            )
        else()
            message(WARNING "DLL not found: ${dll_path}")
        endif()
    endforeach()
endif()
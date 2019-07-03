option(BK_ENABLE_SHADER_TRANSLATOR "Use tranlator from Angle Project to transpile shader" OFF)

# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (BakeryCanvas)
# 查找BakeryCanvas目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(BakeryCanvas DIR_SRCS)
aux_source_directory(BakeryCanvas/jsinternals DIR_SRCS)
aux_source_directory(BakeryCanvas/queue DIR_SRCS)
aux_source_directory(BakeryCanvas/utils DIR_SRCS)
aux_source_directory(BakeryCanvas/2d DIR_SRCS)
aux_source_directory(BakeryCanvas/2d/backend DIR_SRCS)
aux_source_directory(BakeryCanvas/nan DIR_SRCS)

if(NOT BK_ENABLE_SHADER_TRANSLATOR)
list(REMOVE_ITEM DIR_SRCS "BakeryCanvas/translator.h")
list(REMOVE_ITEM DIR_SRCS "BakeryCanvas/translator.cpp")
endif()

link_directories("${PROJECT_SOURCE_DIR}/BakeryCanvas/libs" "/usr/local/lib")
# 指定生成目标
add_executable(BakeryCanvas ${DIR_SRCS} BakeryCanvas/2d/Point.h)
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/BakeryCanvas/include") # 头文件包含目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/BakeryCanvas") # 头文件包含目录

# 编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
# 默认 C 编译器
SET(CMAKE_C_COMPILER "/usr/bin/clang") 
# 默认 C++ 编译器
SET(CMAKE_CXX_COMPILER "/usr/bin/clang") 

# Debug 模式 选项: Release Debug MinSizeRel RelWithDebInfo
SET(CMAKE_BUILD_TYPE "Debug")  
# Debug 模式
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall")  
# Release 模式
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 

# 这段代码可以区分操作系统
MESSAGE("Identifying the OS...")
if(WIN32)
  MESSAGE("This is Windows.")
  target_link_libraries(BakeryCanvas v8_monolith glfw3dll uv)
elseif(APPLE)
  MESSAGE("This is MacOS.")
  target_link_libraries(BakeryCanvas v8_monolith glfw.3 uv cairo png freetype fontconfig pixman-1 c++ "-framework CoreFoundation" "-framework CoreGraphics")
elseif(UNIX)
  MESSAGE("This is Linux.")
endif()

# 编译开关
add_definitions(-DDEBUG)
if(BK_ENABLE_SHADER_TRANSLATOR)
  MESSAGE("Use shader translator")
  target_link_libraries(BakeryCanvas translator preprocessor angle_common)
  add_definitions(-DBK_ENABLE_SHADER_TRANSLATOR)
endif()

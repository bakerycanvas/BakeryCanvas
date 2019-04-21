# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (BakeryCanvas)
# 查找BakeryCanvas目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(BakeryCanvas DIR_SRCS)
link_directories("${PROJECT_SOURCE_DIR}/BakeryCanvas/libs")
# 指定生成目标
add_executable(BakeryCanvas ${DIR_SRCS})
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") # 可执行文件输出目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/BakeryCanvas/include") # 头文件包含目录
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/BakeryCanvas") # 头文件包含目录

# 编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
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
  target_link_libraries(BakeryCanvas v8_monolith glfw3dll)
elseif(APPLE)
  MESSAGE("This is MacOS.")
  target_link_libraries(BakeryCanvas v8_monolith glfw.3 c++)
elseif(UNIX)
  MESSAGE("This is Linux.")
endif()

# 编译开关
add_definitions(-DDEBUG)

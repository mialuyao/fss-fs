cmake_minimum_required(VERSION 3.26)

project(test)

# 如果没有显式指定 build type，默认设置为 Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")


# 设置编译器
set(CMAKE_CXX_COMPILER g++)

include_directories(fss/include)

# 设置编译标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 调试和优化选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer -maes -msse4.2 -fno-strict-aliasing -pedantic -Wall -Wextra -Wunreachable-code -Wmissing-declarations -Wunused-function -Wno-overlength-strings -Wno-deprecated-declarations -O3")

# 设置调试信息标志
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# AESNI 特性开关（等价于 Makefile 中的 ifeq($(AESNI), 1)）
if(DEFINED AESNI AND AESNI)
    add_definitions(-DAESNI)
endif()

# 添加头文件搜索路径
include_directories(fss/include)

link_directories(fss /usr/lib/x86_64-linux-gnu)

# 添加源文件
add_executable(${PROJECT_NAME} test.cpp)

find_library(fss_lib NAMES fss PATHS ./fss)

target_link_libraries(${PROJECT_NAME} ${fss_lib} /usr/lib/x86_64-linux-gnu/libcrypto.a dl ssl gmpxx gmp pthread)

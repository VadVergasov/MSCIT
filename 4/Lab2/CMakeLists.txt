cmake_minimum_required(VERSION 3.16)

project("Lab2" CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

find_package(LLVM REQUIRED CONFIG)

add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

file(GLOB_RECURSE SRC "src/*.hpp" "src/*.h" "src/*.ui" "src/*.cpp")

add_executable(Lab2 ${SRC})

set_property(TARGET Lab2 PROPERTY CXX_STANDARD 20)

target_compile_options(Lab2 PRIVATE -fno-rtti)
target_link_libraries(Lab2 PRIVATE LLVM clangTooling clangFrontend clangParse clangSema clangAnalysis clangAST clangBasic clangDriver clangSerialization clangEdit clangLex)
include_directories(${LLVM_INCLUDE_DIRS})

set_target_properties(Lab2 PROPERTIES AUTOMOC TRUE AUTOUIC TRUE AUTORCC TRUE)

target_link_libraries(Lab2 PRIVATE Qt6::Core Qt6::Widgets Qt::Gui)
